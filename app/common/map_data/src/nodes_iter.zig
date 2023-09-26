const std = @import("std");
const ro = @import("render_objects.zig");

pub const NodesIterator = extern struct {
    iterator: ?*anyopaque,
};

pub fn RO_nodesIter(cobjs: *ro.CMapRenderObjects) callconv(.C) NodesIterator {
    const objs = @as(*ro.MapRenderObjects, @ptrCast(@alignCast(cobjs.ptr.?)));
    var iter: *ro.NODES_HM_T.Iterator = std.heap.c_allocator.create(ro.NODES_HM_T.Iterator) catch |err| {
        ro.logErr("Out of memory: {}\n", .{err});
        return NodesIterator{ .iterator = null };
    };
    iter.* = objs.nodes.iterator();
    return NodesIterator{ .iterator = iter };
}

pub fn RO_destroyNodesIter(citer: *NodesIterator) callconv(.C) void {
    var iter: *ro.NODES_HM_T.Iterator = @ptrCast(@alignCast(citer.iterator));
    std.heap.c_allocator.destroy(iter);
}

pub fn RO_nodesIter_next(citer: *NodesIterator) callconv(.C) ?*ro.Node {
    var iter: *ro.NODES_HM_T.Iterator = @ptrCast(@alignCast(citer.iterator));
    const entry = iter.next();
    if (entry == null) return null;
    var centry = &entry.?.val;
    return centry;
}
