export module init;

import attacks.classical;
import zobrist;

export void feldspar_init()
{
    init_rays_between_squares();
    init_zobrist_hashing();
}
