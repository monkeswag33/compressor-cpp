pub const NodeType = enum(u2) {
    INTERNAL_NODE,
    LEAF_NODE,
    PSEUDO_NODE
};

pub const Node = packed struct {
    node_type: NodeType,
    frequency: usize,
    left: ?*Node,
    right: ?*Node,
    char: u8,
};
