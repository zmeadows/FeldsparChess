#pragma once

class InitFixture : public ::testing::Test {
public:
    InitFixture(){};

    virtual void SetUp();

    virtual void TearDown(){};

    virtual ~InitFixture()
    {
        // cleanup any pending stuff, but no exceptions allowed
    }

private:
    static bool done;
};
