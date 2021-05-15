export module mcts;

import prelude;
import game;
import move;

/*
struct MCTSNode;

// Allocated from the node pool
struct MCTSNodeList {
    MCTSNode* nodes = nullptr;
    U32 count = 0;
};

struct MCTSNode {
    MCTSNodeList children;
    MCTSNode* parent;
    Move move;
    U32 playouts;
    U32 wins;
    F32 uct;
};

struct MCTSNodePool {
    MCTSNode* start = nullptr;
    MCTSNode* tip = nullptr;
    U64 capacity = 0;
    bool overflow
};

static MCTSNodePool NODE_POOL;

void cleanup_node_pool() { NODE_POOL.tip = NODE_POOL.start; }

export void initialize_mtcs(U64 capcity_bytes)
{
    const U64 count = capacity_bytes / sizeof(MCTSNode);
    NODE_POOL.start = static_cast<MCTSNode*>(malloc(sizeof(MCTSNode) * count));
    NODE_POOL.tip = NODE_POOL.start;
    NODE_POOL.capacity = count;
}

MCTSNodeList allocate_nodes(const MoveBuffer& moves)
{
    const U32 nodes_requested = moves.length();
    if (nodes_available(NODE_POOL) >= nodes_requested) {
        MCTSNodeList list;
        list.nodes = NODE_POOL.tip;
        list.count = moves.length();
        NODE_POOL.tip += list.count;

        for (size_t i = 0; i < list.count; i++) {
            MCTSNode& node = list.nodes[i];
        }
        return list;
    } else {
        return {};
    }
}

Move find_best_move_mcts(const Game& game)
{
    MoveBuffer moves;
    generate_moves(game, moves);

    auto __root_children = allocate_nodes(moves);

    if (!__root_children.has_value()) {
        return NULL_MOVE;
    }

    auto root_children = *__root_children;
}
*/
