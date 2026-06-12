#include <gtest/gtest.h>
#include "repository/SampleRepository.h"
#include <filesystem>

// 테스트마다 임시 파일 사용 (격리)
class SampleRepositoryTest : public ::testing::Test {
protected:
    std::string path = "test_samples_tmp.json";
    void TearDown() override { std::filesystem::remove(path); }
    Sample makeSample(const std::string& id, int stock = 100) {
        return {id, "테스트 시료", 1.0, 0.9, stock};
    }
};

TEST_F(SampleRepositoryTest, SaveAndFindById) {
    SampleRepository repo(path);
    repo.save(makeSample("S-001"));
    auto result = repo.findById("S-001");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->sampleId, "S-001");
}

TEST_F(SampleRepositoryTest, FindByIdMissingReturnsNullopt) {
    SampleRepository repo(path);
    EXPECT_FALSE(repo.findById("S-999").has_value());
}

TEST_F(SampleRepositoryTest, UpdateModifiesExisting) {
    SampleRepository repo(path);
    repo.save(makeSample("S-001", 100));
    Sample updated = makeSample("S-001", 200);
    repo.update(updated);
    EXPECT_EQ(repo.findById("S-001")->stock, 200);
}

TEST_F(SampleRepositoryTest, RemoveDeletesItem) {
    SampleRepository repo(path);
    repo.save(makeSample("S-001"));
    repo.remove("S-001");
    EXPECT_FALSE(repo.findById("S-001").has_value());
    EXPECT_TRUE(repo.findAll().empty());
}

TEST_F(SampleRepositoryTest, PersistsAcrossReload) {
    {
        SampleRepository repo(path);
        repo.save(makeSample("S-001"));
    }
    SampleRepository repo2(path);
    EXPECT_TRUE(repo2.findById("S-001").has_value());
}
