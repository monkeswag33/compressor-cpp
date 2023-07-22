const std = @import("std");
const Allocator = std.mem.Allocator;
const log = std.log;
const Node = @import("types.zig").Node;

pub fn freeTree(allocator: Allocator, root: *Node) void {
    if (root.node_type == .INTERNAL_NODE) {
        freeTree(allocator, root.left.?);
        freeTree(allocator, root.right.?);
    }
    allocator.destroy(root);
}

pub fn printTree(allocator: Allocator, root: *Node, level: []const u8) !void {
    switch (root.node_type) {
        .INTERNAL_NODE => {
            log.info("{s}|- I", .{level});
            const level_str = try std.fmt.allocPrint(allocator, "{s}  ", .{level});
            defer allocator.free(level_str);
            try printTree(allocator, root.left.?, level_str);
            try printTree(allocator, root.right.?, level_str);
        },
        .LEAF_NODE => log.info("{s}|- L, \"{c}\"", .{level, root.char}),
        .PSEUDO_NODE => log.info("{s}|- P", .{level})
    }
}