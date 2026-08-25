#include <gtest/gtest.h>
#include <vector>
#include "util/jxx.util.logging.Logger.h"
namespace
{
	using namespace jxx::util::logging; class CaptureHandler final :public Handler
	{
	public:std::vector<jxx::Ptr<LogRecord>> records; void publish(const jxx::Ptr<LogRecord>& r)override
	{
		if (isLoggable(r))records.push_back(r);
	}void flush()override
	{
	}void close()override
	{
	}
	
	
	};

	TEST(LoggerTest, NamedLoggerIsReused)
	{
		auto n = jxx::NEW<jxx::lang::String>("test.named"); EXPECT_EQ(Logger::getLogger(n), Logger::getLogger(n));
	}
	TEST(LoggerTest, LevelFilteringAndHandlerPublishing)
	{
		auto l = Logger::getAnonymousLogger(); auto h = jxx::NEW<CaptureHandler>(); l->addHandler(h); l->setUseParentHandlers(false); l->setLevel(Level::WARNING()); l->info(jxx::NEW<jxx::lang::String>("ignored")); l->warning(jxx::NEW<jxx::lang::String>("kept")); ASSERT_EQ(h->records.size(), 1U); EXPECT_EQ(h->records[0]->getMessage()->utf8(), "kept");
	}
	TEST(LoggerTest, StandardLevelValuesMatchJava)
	{
		EXPECT_EQ(Level::SEVERE()->intValue(), 1000); EXPECT_EQ(Level::WARNING()->intValue(), 900); EXPECT_EQ(Level::INFO()->intValue(), 800); EXPECT_EQ(Level::FINE()->intValue(), 500);
	}
	TEST(LoggerTest, ParentHandlersReceiveRecords)
	{
		auto parent = Logger::getAnonymousLogger(); auto child = Logger::getAnonymousLogger(); auto h = jxx::NEW<CaptureHandler>(); parent->addHandler(h); parent->setUseParentHandlers(false); child->setParent(parent); child->info(jxx::NEW<jxx::lang::String>("message")); EXPECT_EQ(h->records.size(), 1U);
	}
}
