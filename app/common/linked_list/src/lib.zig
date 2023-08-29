const std = @import("std");

pub fn LinkedList(comptime T: type) type {
    return struct {
        head: ?*Self.Node,
        tail: ?*Self.Node,
        allocator: std.mem.Allocator,
        arena: std.heap.ArenaAllocator,

        const Self = @This();

        pub const Node = struct {
            next: ?*Node,
            elem: T,
        };

        pub fn init(allocator: std.mem.Allocator) Self {
            // We use an arena allocator to quickly clear the linked list
            var arena = std.heap.ArenaAllocator.init(allocator);
            return Self{
                .head = null,
                .tail = null,
                .allocator = allocator,
                .arena = arena,
            };
        }

        fn free_elements(self: *Self) void {
            var node: ?*Self.Node = self.head;
            while (node != null) {
                var next_node = node.?.next;
                self.allocator.destroy(node.?);
                node = next_node;
            }
        }

        pub fn deinit(self: *Self) void {
            self.arena.deinit();
        }

        /// Value is copied to the linked list
        pub fn append(self: *Self, value: T) !void {
            const ptr: *Self.Node = try self.allocator.create(Self.Node);
            ptr.*.next = null;
            ptr.*.elem = value;
            if (self.head == null) {
                self.head = ptr;
                self.tail = ptr;
            } else {
                self.tail.?.next = ptr;
                self.tail.? = self.tail.?.next.?;
            }
        }

        /// Clear all elements of the linked list
        pub fn clear(self: *Self) void {
            self.arena.reset(.retain_capacity);
            self.head = null;
            self.tail = null;
        }

        pub fn first(self: *Self) ?*T {
            if (self.head == null) {
                return null;
            } else {
                return &self.head.?.elem;
            }
        }

        pub fn last(self: *Self) ?*T {
            if (self.tail == null) {
                return null;
            } else {
                return &self.tail.?.elem;
            }
        }
    };
}

const test_allocator = std.testing.allocator;
const expect = std.testing.expect;
test "usage" {
    var ll = LinkedList(u64).init(test_allocator);
    try ll.append(1);
    try ll.append(20);
    try ll.append(35);

    try expect(ll.tail.?.elem == 35);

    var elem = ll.head;
    var i: u32 = 0;
    while (elem != null) : (elem = elem.?.next) {
        try switch (i) {
            0 => expect(elem.?.elem == 1),
            1 => expect(elem.?.elem == 20),
            2 => expect(elem.?.elem == 35),
            else => expect(false),
        };
        i += 1;
    }

    ll.deinit();
}
