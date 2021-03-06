#pragma once

#include "common/namespace.hpp"

#include <bam.h>

#ifndef BAM_FSUPPLEMENTAL
# define BAM_FSUPPLEMENTAL 2048
#endif

struct AlignmentFilter {
    struct True {
        bool operator()(bam1_t const* aln) const {
            return true;
        }
    };

    struct False {
        bool operator()(bam1_t const* aln) const {
            return false;
        }
    };

    struct IsPrimary {
        bool operator()(bam1_t const* aln) const {
            return !(aln->core.flag & (BAM_FSECONDARY| BAM_FSUPPLEMENTAL));
        }
    };

    struct IsAligned {
        bool operator()(bam1_t const* aln) const {
            return aln->core.tid >= 0;
        }
    };

    template<typename Combine, typename T1, typename T2>
    struct Chain : private Combine, private T1, private T2 {
        bool operator()(bam1_t const* aln) const {
            return combine()(t1()(aln), t2()(aln));
        }

    private:
        Combine const& combine() const { return *this; }
        T1 const& t1() const { return *this; }
        T2 const& t2() const { return *this; }
    };
};
