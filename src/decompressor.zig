const std = @import("std");
const log = std.log;
const decompressDir = @import("decompression.zig").decompressDir;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();
    defer _ = gpa.deinit();
    var args = std.process.args();
    _ = args.next();
    var arg = args.next();
    if (arg == null) {
        log.err("Please pass the name of the file to decompress", .{});
        std.process.exit(1);
    }
    var filename = arg.?;
    try decompressDir(allocator, filename);
}
