const std = @import("std");

const Library = enum {
    all,
    hashmap,
    linked_list,
    render_objects,
};

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    std.debug.print("Building for {}\n", .{optimize});

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
    render_objects_lib.addIncludePath(.{ .path = "../out/include" });

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
