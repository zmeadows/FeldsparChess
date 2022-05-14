#include "gtest/gtest.h"

#include "init_fixture.h"

#include <mutex>
#include <thread>

static std::mutex g_mutex;

import init;

bool InitFixture::done = false;

void InitFixture::SetUp()
{
    const std::lock_guard<std::mutex> lock(g_mutex);

    if (!InitFixture::done) {
        feldspar_init();
        InitFixture::done = true;
    }
}