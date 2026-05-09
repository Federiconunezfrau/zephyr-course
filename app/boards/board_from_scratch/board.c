#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int board_board_from_scratch_init(void) {
    printf("Board Initialized\n");

    return 0;
}

SYS_INIT(board_board_from_scratch_init, APPLICATION, INIT_LEVEL_ORD(APPLICATION));