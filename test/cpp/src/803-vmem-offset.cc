#include <catch.hpp>
#include "vmem.h"

#include "dram_controller.h"

TEST_CASE("The virtual memory evaluates the correct shift amounts") {
  constexpr unsigned log2_pte_page_size = 12;

  auto level = GENERATE(as<std::size_t>{}, 1,2,3,4,5);

  MEMORY_CONTROLLER dram{1, 3200, 12.5, 12.5, 12.5, 7.5, {}};
  VirtualMemory uut{champsim::data::bytes{1 << log2_pte_page_size}, 5, 200, dram};

  auto expected_value = LOG2_PAGE_SIZE + (log2_pte_page_size-champsim::lg2(PTE_BYTES))*(level-1);
  REQUIRE(uut.shamt(level) == expected_value);
}

TEST_CASE("The virtual memory evaluates the correct offsets") {
  constexpr std::size_t log2_pte_page_size = 12;

  auto level = GENERATE(as<unsigned>{}, 1,2,3,4,5);

  MEMORY_CONTROLLER dram{1, 3200, 12.5, 12.5, 12.5, 7.5, {}};
  VirtualMemory uut{champsim::data::bytes{1 << log2_pte_page_size}, 5, 200, dram};

  champsim::address addr{(0xffff'ffff'ffe0'0000 | (level << LOG2_PAGE_SIZE)) << ((level-1) * 9)};
  REQUIRE(uut.get_offset(addr, level) == level);
}