const std = @import("std");
const Allocator = std.mem.Allocator;
const Wyhash = std.hash.Wyhash;
const meta = std.meta;
const trait = meta.trait;
const autoHash = std.hash.autoHash;
const LinkedList = @import("linked_list").LinkedList;

fn strEq(a: *const []const u8, b: *const []const u8) bool {
    return std.mem.eql(u8, a.*, b.*);
}

pub fn StringHashMap(comptime V: type, comptime cap: usize) type {
    return HashMap([]const u8, V, cap, &strEq);
}

pub fn HashMap(comptime K: type, comptime V: type, comptime cap: usize, comptime eql: *const fn (*const K, *const K) bool) type {
    return struct {
        allocator: Allocator,
        data: [cap]?LinkedList(Self.Entry),
        eql: *const fn (*const K, *const K) bool = eql,

        const Self = @This();

        pub const Error = error{KeyDoesntExist};

        pub const Entry = struct {
            key: K,
            val: V,
        };

        // fn standardEql(a: *K, b: *K) bool {
        //     return a.* == b.*;
        // }

        pub fn init(allocator: Allocator) Self {
            var hm = Self{
                .allocator = allocator,
                .data = undefined,
            };
            @memset(&hm.data, null);
            return hm;
        }

        pub fn deinit(self: *Self) void {
            var i: usize = 0;
            while (i < cap) : (i += 1) {
                if (self.data[i] != null) {
                    self.data[i].?.deinit();
                }
            }
        }

        // pub fn setEqlFn(self: *Self, eql: *const fn (*K, *K) bool) void {
        //     self.eql = eql;
        // }

        fn hash(key: K) u64 {
            if (comptime trait.hasUniqueRepresentation(K)) {
                return Wyhash.hash(0, std.mem.asBytes(&key));
            } else {
                var hasher = Wyhash.init(0);
                // autoHash(&hasher, key);
                std.hash.autoHashStrat(&hasher, key, .Deep); // recursive for more flexible solution
                return hasher.final();
            }
        }

        // The data of val is copied to the hashmap
        pub fn put(self: *Self, key: K, val: V) !void {
            const idx = Self.hash(key) % cap;
            const val_list_ptr = &self.data[idx];
            if (val_list_ptr.* == null) {
                val_list_ptr.* = LinkedList(Self.Entry).init(self.allocator);
            }

            try val_list_ptr.*.?.append(.{ .key = key, .val = val });
        }

        pub fn get(self: *const Self, key: K) ?*V {
            const idx = Self.hash(key) % cap;
            const val_list_ptr: *const ?LinkedList(Self.Entry) = &self.data[idx];
            if (val_list_ptr.* == null) {
                // return Self.Error.KeyDoesntExist;
                return null;
            }

            var node: ?*LinkedList(Self.Entry).Node = val_list_ptr.*.?.head;
            while (node != null) : (node = node.?.next) {
                //if (std.mem.eql(K, &elem.?.elem.key, &key)) {
                // if (node.?.elem.key == key) {
                if (self.eql(&node.?.elem.key, &key)) {
                    return &node.?.elem.val;
                }
            }

            return null;
        }

        pub fn clear(self: *Self) void {
            for (self.data) |elem| {
                if (elem != null) {
                    @constCast(&elem.?).deinit();
                }
            }
        }

        pub const Iterator = struct {
            hm: *Self,
            next_idx: usize,
            current_node: ?*LinkedList(Self.Entry).Node,

            pub fn init(hm: *Self) Self.Iterator {
                return Self.Iterator{
                    .hm = hm,
                    .next_idx = 0,
                    .current_node = null,
                };
            }

            pub fn next(self: *Self.Iterator) ?*Self.Entry {
                if (self.current_node == null) {
                    if (self.next_idx == cap) {
                        return null;
                    }
                    var list = self.hm.data[self.next_idx];
                    self.next_idx += 1;
                    if (list == null) {
                        return self.next();
                    }

                    self.current_node = list.?.head;
                }

                var return_node = &self.current_node.?.elem;
                self.current_node = self.current_node.?.next;

                return return_node;
            }
        };

        pub fn iterator(self: *Self) Self.Iterator {
            return Self.Iterator.init(self);
        }
    };
}

const test_allocator = std.testing.allocator;
const expect = std.testing.expect;
test "usage" {
    var hm = HashMap(u64, u32, 10).init(test_allocator);
    try hm.put(5, 10);
    try hm.put(0, 1);
    try hm.put(11, 2);
    try hm.put(1, 50);
    try hm.put(26553, 54545);

    try expect((hm.get(5)).?.* == 10);
    try expect((hm.get(0)).?.* == 1);
    try expect((hm.get(11)).?.* == 2);
    try expect((hm.get(1)).?.* == 50);
    try expect((hm.get(26553)).?.* == 54545);

    hm.deinit();
}
