#include"verner_utils.hpp"

namespace physapi {
    /*********************************************************************
    *                   Verner Table 1 (BEGIN)
    *********************************************************************/

    void PVernerTable1::loadElement(const std::array<phys_float, VERNERTABLE1_COLS>& row)
    {
        auto z = static_cast<phys_size>(row[eColTab1::Z]);

        if (m_abundances.abundance(eZ(z)) == 0)
        {
            return;
        }

        auto ne = static_cast<phys_size>(row[eColTab1::Ne]);

        if (z != ne)
        {
            return;
        }

        auto is = static_cast<phys_size>(row[eColTab1::Is]);

        auto l = row[eColTab1::L]; // WE DO NOT USE THIS NUMBER!

        auto E_th = row[eColTab1::Eth];
        auto E_0 = row[eColTab1::E0];
        auto sigma_0 = row[eColTab1::SIGMA0];
        auto y_a = row[eColTab1::Ya];
        auto P = row[eColTab1::P];
        auto y_w = row[eColTab1::Yw];

        // static uint is_ = 0;

        if (find({ z, ne, is }) != end())
        {
            (*this)[{z, ne, is}] = { E_th, E_0, sigma_0, y_a, P, y_w };
        }
        else
        {
            uint32_t is_ = is;
            while (find({ z, ne, is }) != end())
            {
                is_++;
            }

            (*this)[{z, ne, is_}] = { E_th, E_0, sigma_0, y_a, P, y_w };
        }
    }

    /*********************************************************************
    *                   Verner Table 1 (END)
    *********************************************************************/


    void PVernerTable2::loadElement(const std::array<phys_float, VERNERTABLE2_COLS>& row)
    {
        auto z = static_cast<phys_size>(row[eColTab2::Z]);

        if (m_abundances.abundance(eZ(z)) == 0)
        {
            return;
        }

        auto ne = static_cast<phys_size>(row[eColTab2::Ne]);

        if (z != ne)
        {
            return;
        }

        auto E_th = row[eColTab2::Eth];
        auto E_max = row[eColTab2::Emax];
        auto E_0 = row[eColTab2::E0];
        auto sigma_0 = row[eColTab2::SIGMA0];
        auto y_a = row[eColTab2::Ya];
        auto P = row[eColTab2::P];
        auto y_w = row[eColTab2::Yw];
        auto y_0 = row[eColTab2::Y0];
        auto y_1 = row[eColTab2::Y1];

        (*this)[{z, ne}] = { E_th, E_max, E_0, sigma_0, y_a, P, y_w, y_0, y_1 };
    }

    void PVernerTable1Hnd::formatShellNumber(t_vect& v) const
    {
        for (phys_size i = v.size(); i > 0; --i)
        {
            v[i - 1].first[2] = getShellNumber(v[i - 1].first[2]);
        }
    }

    phys_size PVernerTable1Hnd::getShellNumber(phys_size is_) const
    {
        static phys_size counter = 0;
        static phys_size previousValue = 0;

        switch (is_)
        {
        case 1:
            counter = 0;
            previousValue = is_;
            return 1;
        case 2:
            counter = 0;
            if (previousValue == 2)
            {
                previousValue = is_;
                return 3;
            }
            else
            {
                previousValue = is_;
                return 2;
            }

        case 3:
            counter++;
            previousValue = is_;
            if (counter == 3)
            {
                counter = 0;
                return 6;
            }
            else if (counter == 2)
            {
                return 5;
            }
            else
            {
                return 4;
            }
        case 4:
            previousValue = is_;
            return 7;
        default:
            return 100; //Error!!
        }
    }
}// namespace physapi