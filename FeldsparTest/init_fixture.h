#pragma once

class InitFixture : public ::testing::Test {
public:
    InitFixture(){};

    void SetUp();

    void TearDown(){};

    ~InitFixture()
    {
        // cleanup any pending stuff, but no exceptions allowed
    }

private:
    static bool done;
};
