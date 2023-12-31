const std = @import("std");
const hashmap_pkg = @import("hashmap");
const HashMap = hashmap_pkg.HashMap;
const Allocator = std.mem.Allocator;
const StringHashMap = std.StringHashMap;
const build_options = @import("build_options");

fn logErr(comptime fmt: []const u8, args: anytype) void {
    if (build_options.is_android) {
        @panic("An error occurred");
    } else {
        std.debug.print(fmt, args);
    }
}

const string = switch (build_options.no_cstd_import) {
    true => struct {
        pub fn strlen(str: [*]const u8) usize {
            var i: usize = 0;
            var val: u8 = str[i];
            while (val != 0) : (i += 1) {
                val = str[i];
            }
            return i - 1;
        }
    },
    false => @cImport({
        @cInclude("string.h");
    }),
};

fn Vec(comptime T: type) type {
    return struct {
        items: []T,
        len: usize,
        allocator: Allocator,

        const Self = @This();

        pub fn init(allocator: Allocator) !Self {
            return try Self.initWithCap(allocator, 100);
        }

        pub fn cap(self: *Self) usize {
            return self.items.len;
        }

        pub fn initWithCap(allocator: Allocator, _cap: usize) !Self {
            return Self{
                .items = try allocator.alloc(T, _cap),
                .len = 0,
                .allocator = allocator,
            };
        }

        pub fn deinit(self: *Self) void {
            self.allocator.free(self.items);
        }

        pub fn append(self: *Self, value: T) !void {
            if (self.len == self.cap()) {
                // self.cap *= 2;
                self.items = try self.allocator.realloc(self.items, self.cap() * 2);
            }

            self.items.ptr[self.len] = value;
            self.len += 1;
        }

        pub fn clear(self: *Self) void {
            self.len = 0;
        }
    };
}

pub const CMapRenderObjects = extern struct {
    ptr: ?*anyopaque,
};

export fn makeRenderObjects() CMapRenderObjects {
    var ptr = std.heap.c_allocator.create(MapRenderObjects) catch |err| {
        logErr("{}\n", .{err});
        return CMapRenderObjects{ .ptr = null };
    };
    var cobjs = CMapRenderObjects{
        .ptr = ptr,
    };
    @as(*MapRenderObjects, @ptrCast(@alignCast(cobjs.ptr.?))).* = MapRenderObjects.init(std.heap.page_allocator) catch |err| {
        logErr("{}\n", .{err});
        std.heap.c_allocator.destroy(@as(*MapRenderObjects, @ptrCast(@alignCast(cobjs.ptr.?))));
        cobjs.ptr = null;
        return cobjs;
    };
    return cobjs;
}

export fn destroyRenderObjects(self: *CMapRenderObjects) void {
    var ptr = @as(*MapRenderObjects, @ptrCast(@alignCast(self.ptr)));
    ptr.*.deinit();
    std.heap.c_allocator.destroy(ptr);
}

export fn RO_getStreets(cobjs: *const CMapRenderObjects, streets: [*c][*]CRO_Street, size: *c_int) void {
    var objs: *MapRenderObjects = @as(*MapRenderObjects, @ptrCast(@alignCast(cobjs.ptr)));
    streets.* = objs.streets.items.ptr;
    size.* = @intCast(objs.streets.len);
}

export fn RO_getNode(cobjs: *const CMapRenderObjects, id: u64) callconv(.C) ?*Node {
    return @as(*MapRenderObjects, @ptrCast(@alignCast(cobjs.ptr))).nodes.get(id);
}

export fn RO_determineBounds(cobjs: *CMapRenderObjects) void {
    var objs: *MapRenderObjects = @as(*MapRenderObjects, @ptrCast(@alignCast(cobjs.ptr)));
    var nodes_iter = objs.nodes.iterator();
    var entry = nodes_iter.next();
    var minlat: i32 = std.math.maxInt(i32);
    var maxlat: i32 = std.math.minInt(i32);
    var minlon: i32 = std.math.maxInt(i32);
    var maxlon: i32 = std.math.minInt(i32);
    while (entry != null) : (entry = nodes_iter.next()) {
        const node: *Node = &entry.?.val;
        if (node.lat < minlat) minlat = node.lat;
        if (node.lat > maxlat) maxlat = node.lat;
        if (node.lon < minlon) minlon = node.lon;
        if (node.lon > maxlon) maxlon = node.lon;
    }
    objs.bounds = .{
        .minlat = minlat,
        .maxlat = maxlat,
        .minlon = minlon,
        .maxlon = maxlon,
    };
}

export fn RO_determineBoundsFromStreets(cobjs: *CMapRenderObjects) void {
    var objs: *MapRenderObjects = @as(*MapRenderObjects, @ptrCast(@alignCast(cobjs.ptr)));
    var minlat: i32 = std.math.maxInt(i32);
    var maxlat: i32 = std.math.minInt(i32);
    var minlon: i32 = std.math.maxInt(i32);
    var maxlon: i32 = std.math.minInt(i32);
    var i: usize = 0;
    while (i < objs.streets.len) : (i += 1) {
        const street = objs.streets.items[i];
        var j: usize = 0;
        while (j < street.nodes_count) : (j += 1) {
            const node: *Node = objs.nodes.get(street.nodes[j]).?;
            if (node.lat < minlat) minlat = node.lat;
            if (node.lat > maxlat) maxlat = node.lat;
            if (node.lon < minlon) minlon = node.lon;
            if (node.lon > maxlon) maxlon = node.lon;
        }
    }
    objs.bounds = .{
        .minlat = minlat,
        .maxlat = maxlat,
        .minlon = minlon,
        .maxlon = maxlon,
    };
}

export fn RO_getBounds(cobjs: *const CMapRenderObjects) *Bounds {
    return &@as(*MapRenderObjects, @ptrCast(@alignCast(cobjs.ptr))).bounds;
}

const CBounds = extern struct {
    minlat: i32,
    maxlat: i32,
    minlon: i32,
    maxlon: i32,
};
const Bounds = CBounds;

const CRO_Street = extern struct {
    type: [*:0]const u8,
    nodes: [*]u64,
    nodes_count: usize,
};

pub const CNode = extern struct {
    id: u64,
    lat: i32,
    lon: i32,
};
pub const Node = CNode;

export fn addNode(cobjs: *CMapRenderObjects, node: Node) void {
    var objs: *MapRenderObjects = @ptrCast(@alignCast(cobjs.ptr.?));
    objs.nodes.put(node.id, node) catch |err| {
        logErr("Couldn't add Node: {}\n", .{err});
    };
}

fn u64Eql(a: *const u64, b: *const u64) bool {
    return a.* == b.*;
}

const NODES_CAP = 1000;
pub const NODES_HM_T = HashMap(u64, Node, NODES_CAP, &u64Eql);
pub const MapRenderObjects = struct {
    allocator: Allocator,
    arena: std.heap.ArenaAllocator,
    streets: Vec(CRO_Street),
    nodes: NODES_HM_T,
    bounds: Bounds,

    const Self = @This();

    fn init(allocator: Allocator) !Self {
        var arena = std.heap.ArenaAllocator.init(allocator);
        var o = Self{
            .allocator = arena.allocator(),
            .arena = arena,
            .streets = undefined,
            .nodes = undefined,
            .bounds = undefined,
        };
        o.streets = try Vec(CRO_Street).init(o.allocator);
        o.nodes = NODES_HM_T.init(o.allocator);
        return o;
    }

    fn deinit(self: *Self) void {
        self.arena.deinit();
    }

    fn addRenderObjectWay(self: *MapRenderObjects, way: *Way) void {
        const highway: ?[]const u8 = way.tags.get("highway");
        if (highway != null) {
            const nodes: []u64 = self.allocator.dupe(u64, way.nodes.items[0..way.nodes.len]) catch |err| {
                logErr("Couldn't allocate nodes array for street: {}\n", .{err});
                return;
            };
            const highway_type: [:0]u8 = self.allocator.dupeZ(u8, highway.?) catch |err| {
                logErr("{}", .{err});
                return;
            };
            var street = CRO_Street{
                .type = highway_type.ptr,
                .nodes = nodes.ptr,
                .nodes_count = nodes.len,
            };

            self.streets.append(street) catch |err| {
                logErr("Couldn't add street: {}\n", .{err});
                return;
            };
        }
    }
};

export fn printObjsSummary(cobjs: *CMapRenderObjects) void {
    var objs: *MapRenderObjects = @ptrCast(@alignCast(cobjs.ptr.?));
    logErr("RenderObjects: streetscount={}, nodescount={} \n  Bounds={}\n", .{ objs.streets.len, objs.nodes.len(), objs.bounds });
}

export fn printObjs(cobjs: *CMapRenderObjects) void {
    var objs: *MapRenderObjects = @ptrCast(@alignCast(cobjs.ptr.?));
    logErr("RenderObjects: streetscount={}, nodescount={} \n  Bounds={}\n", .{ objs.streets.len, objs.nodes.len(), objs.bounds });
    var i: u32 = 0;
    while (i < objs.streets.len) : (i += 1) {
        logErr("  Street: {}\n", .{objs.streets.items[i]});
    }
}

const WAY_TAGS_CAP = 50;
const Way = struct {
    id: u64,
    nodes: Vec(u64),
    tags: StringHashMap([]const u8),
    allocator: Allocator,
    arena: std.heap.ArenaAllocator,
};

export fn initWay() ?*anyopaque {
    var way_ptr = std.heap.c_allocator.create(Way) catch |err| {
        logErr("{}\n", .{err});
        return null;
    };
    var cway: *anyopaque = way_ptr;
    var way: *Way = @ptrCast(@alignCast(cway));
    way.arena = std.heap.ArenaAllocator.init(std.heap.c_allocator);
    way.allocator = way.arena.allocator();
    way.nodes = Vec(u64).init(std.heap.c_allocator) catch |err| {
        logErr("Couldn't init way (nodes couldn't be allocated): {}\n", .{err});
        return null;
    };
    way.tags = StringHashMap([]const u8).init(std.heap.c_allocator);
    return cway;
}

export fn freeWay(cway: *anyopaque) void {
    var way: *Way = @ptrCast(@alignCast(cway));
    way.arena.deinit();
    way.tags.deinit();
    way.nodes.deinit();
    std.heap.c_allocator.destroy(way);
}

export fn setWay(cway: *anyopaque, id: u64) void {
    var way: *Way = @ptrCast(@alignCast(cway));
    way.id = id;
    way.nodes.clear();
    way.tags.clearRetainingCapacity();
    _ = way.arena.reset(.retain_capacity);
}

export fn way_addNode(cway: *anyopaque, id: u64) void {
    var way: *Way = @ptrCast(@alignCast(cway));
    way.nodes.append(id) catch |err| {
        logErr("Couldn't add Node: {}\n", .{err});
    };
}

export fn way_addTag(cway: *anyopaque, key: [*c]const u8, val: [*c]const u8) void {
    var way: *Way = @ptrCast(@alignCast(cway));

    const k = way.allocator.alloc(u8, string.strlen(key)) catch |err| {
        logErr("{}\n", .{err});
        return;
    };
    const v = way.allocator.alloc(u8, string.strlen(val)) catch |err| {
        logErr("{}\n", .{err});
        return;
    };

    @memcpy(k, @as([*]const u8, @ptrCast(key)));
    @memcpy(v, @as([*]const u8, @ptrCast(val)));

    way.tags.put(k, v) catch |err| {
        logErr("Couldn't put tag: {}\n", .{err});
    };
}

export fn wayToRenderObject(cobjs: *CMapRenderObjects, cway: *anyopaque) void {
    var objs: *MapRenderObjects = @ptrCast(@alignCast(cobjs.ptr.?));
    var way: *Way = @ptrCast(@alignCast(cway));

    objs.addRenderObjectWay(way);
}

//=== Iterator ===//
const NodesIterator = extern struct {
    iterator: ?*anyopaque,
};

export fn RO_nodesIter(cobjs: *CMapRenderObjects) callconv(.C) NodesIterator {
    const objs = @as(*MapRenderObjects, @ptrCast(@alignCast(cobjs.ptr.?)));
    var iter: *NODES_HM_T.Iterator = std.heap.c_allocator.create(NODES_HM_T.Iterator) catch |err| {
        logErr("Out of memory: {}\n", .{err});
        return NodesIterator{ .iterator = null };
    };
    iter.* = objs.nodes.iterator();
    return NodesIterator{ .iterator = iter };
}

export fn RO_destroyNodesIter(citer: *NodesIterator) callconv(.C) void {
    var iter: *NODES_HM_T.Iterator = @ptrCast(@alignCast(citer.iterator));
    std.heap.c_allocator.destroy(iter);
}

export fn RO_nodesIter_next(citer: *NodesIterator) callconv(.C) ?*Node {
    var iter: *NODES_HM_T.Iterator = @ptrCast(@alignCast(citer.iterator));
    const entry = iter.next();
    if (entry == null) return null;
    var centry = &entry.?.val;
    return centry;
}
