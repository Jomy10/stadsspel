const std = @import("std");
const builtin = @import("builtin");
const AndroidSdk = @import("Sdk.zig");

const Library = enum {
    all,
    hashmap,
    linked_list,
    render_objects,
};

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    std.debug.print("Building for optimization level: {}\n", .{optimize});
    std.debug.print("Building for {any} {any}\n", .{ target.cpu_arch, target.os_tag });

    const darwin = if (target.os_tag) |tag| tag.isDarwin() else false;
    const android = (target.abi orelse .none) == .android;

    var android_sdk: *AndroidSdk = undefined;
    var android_version = b.option(AndroidSdk.AndroidVersion, "android-version", "the android version, default is android13") orelse .android13;
    _ = android_version;
    if (android) {
        std.debug.print("Linking SDK for Android\n", .{});
        android_sdk = AndroidSdk.init(b, null, .{});
    }

    var ios_sdk_path: ?[]const u8 = null;
    if (darwin) {
        std.debug.print("Linking sysroot for Darwin system\n", .{});
        ios_sdk_path = if (b.sysroot) |sysroot| sysroot else switch (builtin.os.tag) {
            .macos => blk: {
                const target_info = std.zig.system.NativeTargetInfo.detect(target) catch
                    @panic("Couldn't detect native target info");
                const sdk = std.zig.system.darwin.getSdk(b.allocator, target_info.target) orelse
                    @panic("Couldn't detect Apple SDK");
                break :blk sdk;
            },
            else => {
                @panic("Missing path to Apple SDK");
            },
        };
        b.sysroot = ios_sdk_path.?;
    }

    var build_options = b.addOptions();
    build_options.addOption(bool, "no_cstd_import", if (target.os_tag) |tag| switch (tag) {
        .ios => true, // problems linking to C standarrd library
        else => switch (target.abi orelse .none) {
            .android => true,
            else => false,
        },
    } else false);
    build_options.addOption(bool, "alt_panic", if (target.os_tag) |tag| switch (tag) {
        .ios => true,
        else => false,
    } else false);
    build_options.addOption(bool, "is_android", android);

    //=== options ===//
    const compile_c = b.option(bool, "compile-c", "Compile to a library usable from C") orelse false;
    const lib_opt = std.meta.stringToEnum(Library, b.option([]const u8, "lib", "The library to compile. Specify none to compile all") orelse "all").?;

    //=== Sources ===//
    const ll_root = "linked_list/src/lib.zig";
    const hm_root = "hashmap/src/lib.zig";
    const render_objects_root = "map_data/src/render_objects.zig";

    //=== Modules ===//
    const ll_mod = b.addModule("linked_list", .{ .source_file = .{ .path = ll_root } });
    const hm_mod = b.addModule("hashmap", .{
        .source_file = .{ .path = hm_root },
        .dependencies = &.{
            .{ .name = "linked_list", .module = ll_mod },
        },
    });

    //=== Static libs ===//
    const ll_lib = b.addStaticLibrary(.{
        .name = "linked_list",
        .root_source_file = .{ .path = ll_root },
        .target = target,
        .optimize = optimize,
    });

    const hm_lib = b.addStaticLibrary(.{
        .name = "zhashmap",
        .root_source_file = .{ .path = hm_root },
        .target = target,
        .optimize = optimize,
    });
    hm_lib.addModule("linked_list", ll_mod);

    const render_objects_lib = b.addStaticLibrary(.{
        .name = "render_objects",
        .root_source_file = .{ .path = render_objects_root },
        .target = target,
        .optimize = optimize,
    });
    render_objects_lib.addModule("hashmap", hm_mod);
    render_objects_lib.addModule("build_options", build_options.createModule());
    render_objects_lib.addIncludePath(.{ .path = "../out/include" });
    // darwin platforms (TODO: check and only include of target is darwin (simulator, ios, macos, tvos, ...))
    // const sdk = std.zig.system.darwin.getSdk(b.allocator, target.toTarget()).?;
    // defer sdk.deinit(allocator);
    if (darwin) {
        render_objects_lib.addSystemIncludePath(.{ .path = ios_sdk_path.? });
    }
    if (android) {
        render_objects_lib.link_emit_relocs = true;
        render_objects_lib.link_eh_frame_hdr = true;
        render_objects_lib.force_pic = true;
        render_objects_lib.link_function_sections = true;
        render_objects_lib.bundle_compiler_rt = true;
        render_objects_lib.strip = (optimize == .ReleaseSmall);
        render_objects_lib.export_table = true;
        render_objects_lib.defineCMacro("ANDROID", null);
        render_objects_lib.linkLibC();
    }

    // render_objects_lib.addFrameworkPath(.{ .path = b.pathJoin(&.{ b.sysroot.?, "/System/Library/Frameworks" }) });
    // render_objects_lib.addSystemIncludePath(.{ .path = b.pathJoin(&.{ b.sysroot.?, "/usr/include" }) });
    // render_objects_lib.addLibraryPath(.{ .path = b.pathJoin(&.{ b.sysroot.?, "/usr/lib" }) });

    const libs: [3]*std.Build.Step.Compile = .{ ll_lib, hm_lib, render_objects_lib };
    if (compile_c) {
        for (libs) |lib| {
            switch (optimize) {
                .Debug, .ReleaseSafe => lib.bundle_compiler_rt = true,
                .ReleaseFast, .ReleaseSmall => lib.disable_stack_probing = true,
            }
            lib.force_pic = true;
        }
    }

    switch (lib_opt) {
        .all => for (libs) |lib| {
            std.debug.print("Install {s}\n", .{lib.name});
            b.installArtifact(lib);
        },
        .hashmap => b.installArtifact(hm_lib),
        .linked_list => b.installArtifact(ll_lib),
        .render_objects => b.installArtifact(render_objects_lib),
    }

    //=== Tests ===//
    const ll_tests = b.addTest(.{
        .root_source_file = .{ .path = ll_root },
        .target = target,
        .optimize = optimize,
    });

    const hm_tests = b.addTest(.{
        .root_source_file = .{ .path = hm_root },
        .target = target,
        .optimize = optimize,
    });
    hm_tests.addModule("linked_list", ll_mod);

    const render_objects_tests = b.addTest(.{
        .root_source_file = .{ .path = render_objects_root },
        .target = target,
        .optimize = optimize,
    });
    render_objects_tests.addModule("hashmap", hm_mod);
    render_objects_tests.addModule("linked_list", ll_mod);

    const run_ll_tests = b.addRunArtifact(ll_tests);
    const run_hm_tests = b.addRunArtifact(hm_tests);
    const run_ro_tests = b.addRunArtifact(render_objects_tests);

    const test_step = b.step("test", "Run library tests");
    // test_step.dependOn(&run_ll_tests.step);
    // test_step.dependOn(&run_hm_tests.step);
    // test_step.dependOn(&run_ro_tests.step);

    const tests: [3]*std.Build.Step.Run = .{ run_ll_tests, run_hm_tests, run_ro_tests };
    switch (lib_opt) {
        .all => for (tests) |t| {
            // std.debug.print("Install {s}\n", .{lib.name});
            test_step.dependOn(&t.step);
        },
        .hashmap => test_step.dependOn(&run_hm_tests.step),
        .linked_list => test_step.dependOn(&run_ll_tests.step),
        .render_objects => test_step.dependOn(&run_ro_tests.step),
    }
}
