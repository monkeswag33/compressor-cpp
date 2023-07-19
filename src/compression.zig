const types = @import("types.zig");
const Node = types.Node;
const NodeType = types.NodeType;
const std = @import("std");
const fs = std.fs;
const log = std.log;
const Allocator = std.mem.Allocator;
const PriorityQueue = std.PriorityQueue(Node, void, lessThan);

pub fn compress_dir(allocator: Allocator, dir_name: []const u8, out_file: *fs.File) !void {
    const writer = out_file.writer();
    try out_file.writeAll(dir_name);
    try writer.writeByte(0);
    const dir = try fs.openDirAbsolute(dir_name, .{});
    var iter = (try dir.openIterableDir(".", .{})).iterate();
    const num_files_offset = try out_file.getPos();
    try out_file.seekBy(@sizeOf(u32));
    var num_files: u32 = 0;
    while (try iter.next()) |e| : (num_files += 1) {
        var file = try dir.openFile(e.name, .{});
        defer file.close();
        try compress_file(allocator, e.name, file, out_file, writer);
    }
    try out_file.seekTo(num_files_offset);
    try writer.writeIntLittle(u32, num_files);
    log.info("Found {} files", .{num_files});
}

fn lessThan(context: void, a: Node, b: Node) std.math.Order {
    _ = context;
    return std.math.order(a.frequency, b.frequency);
}

fn compress_file(allocator: Allocator, filename: []const u8, file: fs.File, out_file: *fs.File, writer: fs.File.Writer) !void {
    _ = out_file;
    try writer.writeAll(filename);
    try writer.writeByte(0);
    var pq = PriorityQueue.init(allocator, {});
    defer pq.deinit();
    try pq.add(.{
        .node_type = NodeType.PSEUDO_NODE,
        .left = null,
        .right = null,
        .frequency = 0,
        .char = 0
    });
    try read_file(allocator, file, &pq);
}

fn read_file(allocator: Allocator, file: fs.File, pq: *PriorityQueue) !void {
    log.info("Reading file", .{});
    var buffer = try allocator.alloc(u8, 10);
    defer allocator.free(buffer);
    var l: usize = try file.readAll(buffer);
    var chars = std.AutoHashMap(u8, usize).init(allocator);
    defer chars.deinit();
    while (l > 0) : (l = try file.readAll(buffer)) {
        for (buffer[0..l]) |c| {
            const v = try chars.getOrPut(c);
            if (v.found_existing) {
                v.value_ptr.* += 1;
            } else {
                v.value_ptr.* = 1;
            }
        }
    }
    var iter = chars.keyIterator();
    while (iter.next()) |c| {
        try pq.add(.{
            .node_type = NodeType.LEAF_NODE,
            .frequency = chars.get(c.*).?,
            .left = null,
            .right = null,
            .char = c.*
        });
    }
    var iter2 = pq.iterator();
    while (iter2.next()) |e| {
        log.info("{}", .{e});
    }
}
