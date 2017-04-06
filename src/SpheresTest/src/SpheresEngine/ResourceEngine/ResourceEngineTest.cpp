#include <SpheresEngine/ResourceEngine/ResourceEngine.h>

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

TEST(ResourceEngineTest, fileWatch) {

	auto tmpFolder = boost::filesystem::temp_directory_path();

	boost::filesystem::path filename("foo.txt");
	auto full_path = tmpFolder / filename;

	// write to file
	{
		std::ofstream fout(full_path.native(), std::ofstream::out);
		fout << "Some Content";
	}

	ResourceEngine resEngine(true);
	auto text = resEngine.loadTextFile(full_path.native());

	ASSERT_TRUE(
			resEngine.getChangedFiles().find(full_path.native())
					== resEngine.getChangedFiles().end());

	//EXPECT_THAT(resEngine.getChangedFiles(), Not(Contains(full_path.native())));
	{
		std::ofstream fout(full_path.native(),
				std::ofstream::out | std::ofstream::app);
		fout << "Some More Content";
	}

	ASSERT_TRUE(
			resEngine.getChangedFiles().find(full_path.native())
					!= resEngine.getChangedFiles().end());
	{
		std::ofstream fout(full_path.native(),
				std::ofstream::out | std::ofstream::app);
		fout << "Some More Content - to go";
	}

	ASSERT_TRUE(
			resEngine.getChangedFiles().find(full_path.native())
					!= resEngine.getChangedFiles().end());
}
