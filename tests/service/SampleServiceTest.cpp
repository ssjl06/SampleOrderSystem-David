#include <gtest/gtest.h>
#include "service/SampleService.h"
#include <filesystem>

class SampleServiceTest : public ::testing::Test {
protected:
    std::string path = "test_svc_samples_tmp.json";
    void TearDown() override { std::filesystem::remove(path); }

    SampleRepository makeRepo() { return SampleRepository(path); }
    Sample makeSample(const std::string& id) {
        return {id, "테스트 시료", 1.0, 0.9, 100};
    }
};

TEST_F(SampleServiceTest, RegisterSampleSucceeds) {
    SampleRepository repo(path);
    SampleService svc(repo);
    svc.registerSample(makeSample("S-001"));
    EXPECT_EQ(svc.getAllSamples().size(), 1u);
}

TEST_F(SampleServiceTest, DuplicateSampleIdThrows) {
    SampleRepository repo(path);
    SampleService svc(repo);
    svc.registerSample(makeSample("S-001"));
    EXPECT_THROW(svc.registerSample(makeSample("S-001")), std::invalid_argument);
}

TEST_F(SampleServiceTest, FindByIdReturnsCorrectSample) {
    SampleRepository repo(path);
    SampleService svc(repo);
    svc.registerSample(makeSample("S-001"));
    auto result = svc.findById("S-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->sampleId, "S-001");
}

TEST_F(SampleServiceTest, SearchByNameFindsMatch) {
    SampleRepository repo(path);
    SampleService svc(repo);
    svc.registerSample({"S-001", "실리콘 웨이퍼", 1.0, 0.9, 100});
    svc.registerSample({"S-002", "갈륨비소 기판", 1.0, 0.9, 100});
    auto result = svc.searchByName("실리콘");
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].sampleId, "S-001");
}

TEST_F(SampleServiceTest, SearchByNameReturnsEmptyWhenNoMatch) {
    SampleRepository repo(path);
    SampleService svc(repo);
    svc.registerSample({"S-001", "실리콘 웨이퍼", 1.0, 0.9, 100});
    EXPECT_TRUE(svc.searchByName("없는키워드").empty());
}
