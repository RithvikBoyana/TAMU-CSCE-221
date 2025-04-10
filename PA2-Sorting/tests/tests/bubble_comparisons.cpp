#include "executable.h"
#include "memorize.h"
#include <algorithm>
#include <vector>

TEST(bubble_comparisons) {
    Typegen t;

    for(size_t sz = 0; sz < 800; sz++) {
        std::vector<double> vec(sz);
        std::vector<double> gt(sz);

        t.fill(vec.begin(), vec.end());
        std::copy(vec.begin(), vec.end(), gt.begin());

        size_t comparisons = 0;

        sort::bubble(vec.begin(), vec.end(), [&comparisons](double & i, double & j) {
            comparisons++;
            return i > j;
        });

        std::sort(gt.begin(), gt.end(), [](double & i, double & j) { return i > j; });

        bool equal = std::equal(vec.begin(), vec.end(), gt.begin());
        if (!equal) {
            std::cerr << "ERROR: Vector is not sorted!\n"
                    << "\tAre you using the Comparator comp?\n";
            if (sz < 30) {
                std::cerr << "\tYour Vector: " << vec
                        << "\n\tCorrect Vector: " << gt << '\n';
            } else {
                std::cerr << "\tVectors are too large to print.\n";
            }
        }
        ASSERT_TRUE(equal);

        if (comparisons < memorize::bubble_comparisons[sz] - memorize::bubble_error(sz)) {
            std::cerr << "ERROR: Comparisons (" << comparisons << ") is fewer than expected.\n"
                    << "\tDid you implement Bubble Sort?" << std::endl;
        } else if (memorize::bubble_comparisons[sz] + memorize::bubble_error(sz) < comparisons) {
            std::cerr << "ERROR: Comparisons (" << comparisons << ") is greater than expected.\n"
                    << "\tYou should make your algorithm more efficient.\n"
                    << "\tHINT: Do not iterate over elements that are already sorted." << std::endl;
        }
        ASSERT_NEAR(memorize::bubble_comparisons[sz], comparisons, memorize::bubble_error(sz));

        // Check if best case is O(n)
        if (sz > 2) {
            std::rotate(vec.begin(), vec.end() - 1, vec.end()); // Should result in only 2 passes in optimized bubble sort
            
            comparisons = 0;
            sort::bubble(vec.begin(), vec.end(), [&comparisons](double & i, double & j) {
                comparisons++;
                return i > j;
            });

            if (comparisons < memorize::bubble_comparisons_best[sz] - memorize::bubble_best_error(sz)) {
                std::cerr << "ERROR: Comparisons (" << comparisons << ") is fewer than expected.\n"
                        << "\tDid you implement Bubble Sort?" << std::endl;
            } else if (memorize::bubble_comparisons_best[sz] + memorize::bubble_best_error(sz) < comparisons) {
                std::cerr << "ERROR: Comparisons (" << comparisons << ") is greater than expected.\n"
                        << "\tYou should make your algorithm more efficient.\n"
                        << "\tHINT: Your bubble sort must be optimized to be O(n) in the best case." << std::endl;
            }
            ASSERT_NEAR(memorize::bubble_comparisons_best[sz], comparisons, memorize::bubble_best_error(sz));
        }
    }
}
