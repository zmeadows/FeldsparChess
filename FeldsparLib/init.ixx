export module init;

import attacks.pext;
import zobrist;

export void feldspar_init()
{
    init_zobrist_hashing();
    init_pext_tables();
}
