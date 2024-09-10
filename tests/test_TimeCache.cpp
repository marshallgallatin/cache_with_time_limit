
#include <TimeCache.h>
#include <gtest/gtest.h>

#include <chrono>
#include <thread>
#include <sstream>
#include <vector>
#include <string>
#include <stdint.h>
#include <assert.h>
#include <memory>

enum class Actions
{
    TIME_LIMITED_CACHE,
    SET,
    GET,
    COUNT
};

struct TimeCacheParams
{
    std::vector<Actions> actions;
    std::vector<std::vector<int>> values;
    std::vector<uint32_t> timeDelays;
    std::vector<std::string> outputs;
};

class TimeCacheFixture : public ::testing::TestWithParam<TimeCacheParams>
{
    protected:
        TimeCacheFixture()
        {};

    public:
        void testOutput(std::string expected, bool actual)
        {
            bool expectedBool;
            std::istringstream(expected) >> std::boolalpha >> expectedBool;
            EXPECT_EQ(expectedBool, actual);
        }

        void testOutput(std::string expected, int actual)
        {
            int expectedInt;
            std::istringstream(expected) >> expectedInt;
            EXPECT_EQ(expectedInt, actual);
        }

        void performAction(Actions action, std::vector<int> values, uint32_t timeDelay, std::string output)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeDelay));

            switch(action)
            {
                case Actions::TIME_LIMITED_CACHE:
                    assert(values.size() == 0 && "values must be empty for creating the TimeCache");
                    theTimeCache = std::make_unique<TimeCache>();
                    break;
                case Actions::SET:
                    assert(values.size() == 3 && "values must be size 3 for the set action");
                    testOutput(
                        output,
                        theTimeCache->set(values[0], values[1], values[2])
                    );

                    break;
                case Actions::GET:
                    assert(values.size() == 1 && "values must be size 1 for the get action");
                    testOutput(
                        output,
                        theTimeCache->get(values[0])
                    );
                    break;
                case Actions::COUNT:
                    assert(values.size() == 0 && "values must be empty for the count action");
                    testOutput(
                        output,
                        theTimeCache->count()
                    );
                    break;
                default:
                    assert(false && "ACTION is not implemented, implement here");
            }
        }

    private:
        std::unique_ptr<TimeCache> theTimeCache;
};

TimeCacheParams testParams[] = 
{   // actions, values, timeDelays, output
    {
        {Actions::TIME_LIMITED_CACHE, Actions::SET, Actions::GET, Actions::COUNT, Actions::GET},                
        {{}, {1, 42, 110}, {1}, {}, {1}},                   
        {0, 0, 50, 0, 100},            
        {"null", "false", "42", "1", "-1"}
    },
    {
        {Actions::TIME_LIMITED_CACHE, Actions::SET, Actions::SET, Actions::GET, Actions::GET, Actions::GET, Actions::COUNT},  
        {{}, {1, 42, 50}, {1, 50, 100}, {1}, {1}, {1}, {}}, 
        {0, 0, 40, 10, 70, 80, 50},  
        {"null", "false", "true", "50", "50", "-1", "0"}
    }
};

TEST_P(TimeCacheFixture, does_nothing)
{
    EXPECT_EQ(1, 1);
}

TEST_P(TimeCacheFixture, run_test_case)
{
    const TimeCacheParams& testParams = GetParam();
    for(size_t i = 0; i < testParams.actions.size(); ++i)
    {
        performAction(testParams.actions[i], testParams.values[i], testParams.timeDelays[i], testParams.outputs[i]);
    }
    EXPECT_EQ(1, 1);
}


INSTANTIATE_TEST_SUITE_P
(
    TimeCacheTests,
    TimeCacheFixture,
    ::testing::ValuesIn(testParams)
);

