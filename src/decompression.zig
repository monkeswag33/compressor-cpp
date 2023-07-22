const std = @import("std");
const Allocator = std.mem.Allocator;
const log = std.log;
const fs = std.fs;
const types = @import("types.zig");
const util = @import("util.zig");
const Node = types.Node;
const NodeType = types.NodeType;

inline fn readString(reader: *fs.File.Reader, buffer: *std.ArrayList(u8), optional_max_size: ?usize) ![]const u8 {
    buffer.clearAndFree();
    try reader.streamUntilDelimiter(buffer.writer(), 0, optional_max_size);
    return buffer.items;
}

pub fn decompressDir(allocator: Allocator, in_file: []const u8) !void {
    var file = try fs.cwd().openFile(in_file, .{});
    defer file.close();
    var reader = file.reader();
    var buffer = std.ArrayList(u8).init(allocator);
    defer buffer.deinit();
    const dir_name = try readString(&reader, &buffer, 255);
    try fs.cwd().makePath(dir_name);
    var dir = try fs.cwd().openDir(dir_name, .{});
    const num_files = try reader.readIntNative(u32);
    for (0..num_files) |i| {
        _ = i;
        const filename = try readString(&reader, &buffer, 255);
        log.info("Decompressing {s}", .{filename});
        var out_file = try dir.createFile(filename, .{});
        defer out_file.close();
        var writer = out_file.writer();
        try decompressFile(allocator, &writer, &reader);
    }
    log.info("Decompressed {} files", .{num_files});
}

fn decompressFile(allocator: Allocator, writer: *fs.File.Writer, reader: *fs.File.Reader) !void {
    const root = try readTree(allocator, reader);
    defer util.freeTree(allocator, root);
    try readFile(root, reader, writer);
}

fn readTree(allocator: Allocator, reader: *fs.File.Reader) !*Node {
    const node_type: NodeType = @enumFromInt(try reader.readByte());
    const node = try allocator.create(Node);
    node.* = .{
        .frequency = 0,
        .char = 0,
        .node_type = node_type,
        .left = null,
        .right = null,
    };
    if (node_type == .INTERNAL_NODE) {
        node.left = try readTree(allocator, reader);
        node.right = try readTree(allocator, reader);
    } else if (node_type == .LEAF_NODE)
        node.char = try reader.readByte();
    return node;
}

fn readFile(tree: *Node, reader: *fs.File.Reader, writer: *fs.File.Writer) !void {
    var cur_node = tree;
    while (reader.readByte()) |byte| {
        var b = byte;
        for (0..8) |_| {
            cur_node = if ((b & 1) == 1) cur_node.right.? else cur_node.left.?;
            if (cur_node.node_type == .LEAF_NODE) {
                try writer.writeByte(cur_node.char);
                cur_node = tree;
            } else if (cur_node.node_type == .PSEUDO_NODE)
                return;
            b >>= 1;
        }
    } else |_| {}
}
