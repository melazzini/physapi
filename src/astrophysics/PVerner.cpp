#include "PVerner.hpp"
#include "verner_utils.hpp"

namespace physapi
{
    const std::string PVerner::DEFAULT_FILE_PATH_TABLE1 = "./physapi/include/astrophysics/tables/vernertable1.txt";
    const std::string PVerner::DEFAULT_FILE_PATH_TABLE2 = "./physapi/include/astrophysics/tables/vernertable2.txt";

    phys_float PVerner::operator()(phys_float energy)
    {
        //first we reset the value of m_sigma
        m_sigma = 0;

        //the energy units in physapi are joules and
        //because the algorithm works in eV
        //we need to convert the energy to eV
        energy /= 1.0_eV;

        //now we iterate through all the elements in m_Table1,
        //for each element we calculate the corresponding
        //cross section and then we add that value to m_sigma
        //but before we add that value we need to multiply it
        //with the abundance of the corresponding element
        for (auto it_Table1 = m_table1.cbegin(); it_Table1 != m_table1.cend(); ++it_Table1)
        {

            //here we pass a pointer(iterator) to the current element
            //of m_Table1, also we need to pass the photon energy
            m_sigma += (getSigma(it_Table1, energy) * m_abundances.abundance(eZ(it_Table1->first[0])));
        }

        //the returned sigma is not complete yet, according to
        //Verner et al 1996, we need to multiply it by Mb=10^-18 cm^2
        m_sigma *= 1.0E-18;

        //we need to convert the units back to SI
        return m_sigma * 1.0_cm2;
    }

    phys_float PVerner::operator()(phys_float energy,
                                   std::vector<std::array<phys_float, 4>> &listFromVerner)
    {

        //first we reset the value of m_sigma
        m_sigma = 0;

        //the energy units in physapi are joules and
        //because the algorithm works in eV
        //we need to convert the energy to eV
        energy /= 1.0_eV;

        phys_float sigma_i{};

        //now we iterate through all the elements in m_Table1,
        //for each element we calculate the corresponding
        //cross section and then we add that value to m_sigma
        //but before we add that value we need to multiply it
        //with the abundance of the corresponding element
        for (auto it_Table1 = m_table1.cbegin(); it_Table1 != m_table1.cend(); ++it_Table1)
        {
            //here we pass a pointer(iterator) to the current element
            //of m_Table1, also we need to pass the photon energy
            sigma_i = getSigma(it_Table1, energy) *
                      m_abundances.abundance(eZ(it_Table1->first[static_cast<phys_size>(eTable1Key::Z)]));

            m_sigma += sigma_i;

            listFromVerner.push_back({
                static_cast<phys_float>(it_Table1->first[static_cast<phys_size>(eTable1Key::Z)]),
                static_cast<phys_float>(it_Table1->first[static_cast<phys_size>(eTable1Key::Ne)]),
                static_cast<phys_float>(it_Table1->first[static_cast<phys_size>(eTable1Key::Is)]),
                static_cast<phys_float>(sigma_i * 1.0_Mb) * 1.0_cm2 // THIS IS THE SIGMA FOR THE CURRENT ELEMENT
            });
        }

        //the returned sigma is not complete yet, according to
        //Verner et al 1996, we need to multiply it by Mb=10^-18 cm^2
        // m_sigma *= 1.0E-18;

        //we need to convert the units back to SI
        return m_sigma * 1.0_Mb * 1.0_cm2;
    }

    int i = 0;

    phys_float PVerner::getSigma(const PVerner::Table1::const_iterator &it_Table1, phys_float e)
    {
        //in this implementation we will follow
        //the Prof Verner' Algorithm, that was
        //written in fortran, and for the seek
        //of clarity we will show step by step
        //the original code in fortran, there
        //may be some steps that we can avoid
        //in future implementations

        //the next lines are made by us, trying to
        //improve the efficiency of the algorithm

        //we set the values of the variables
        auto nz = it_Table1->first[(uint32_t)eTable1Key::Z];
        auto ne = it_Table1->first[(uint32_t)eTable1Key::Ne];
        auto is = it_Table1->first[(uint32_t)eTable1Key::Is];

        //we need to update the array to
        //select the correct address of
        //the current element in m_Table2

        CurrentElementTable2[(uint32_t)eTable2Key::Z] = nz;
        CurrentElementTable2[(uint32_t)eTable2Key::Ne] = ne;

        //we need a reference to the current element in m_Table2
        const auto &element_table2 = m_table2[{CurrentElementTable2}];

        //here begin the actual algorithm

        //if(nz.lt.1.or.nz.gt.30)return
        if (nz < 1 || nz > 30)
            return 0;

        //if(ne.lt.1.or.ne.gt.nz)return
        if (ne < 1 || ne > nz)
            return 0;

        nout = ntot[ne - 1];

        //if(nz.eq.ne.and.nz.gt.18)nout=7
        if (nz == ne && nz > 18)
            nout = 7;

        //if(nz.eq.(ne+1).and.(nz.eq.20.or.nz.eq.21.or.nz.eq.22.or.nz.eq.25.or.nz.eq.26))
        if (nz == (ne + 1) &&
            (nz == 20 || nz == 21 || nz == 22 || nz == 25 || nz == 26))
            nout = 7;
        //if(is.gt.nout)return
        if (is > nout)
            return 0;

        //if(e.lt.ph1(1,nz,ne,is))return
        if (e < it_Table1->second[0])
            return 0;

        //nint=ninn(ne)
        nint = ninn[ne - 1];

        // if(nz.eq.15.or.nz.eq.17.or.nz.eq.19.or.
        //   &(nz.gt.20.and.nz.ne.26))then
        //      einn=0.0
        // else
        //      if(ne.lt.3)then
        //         einn=1.0e+30
        //      else
        //         einn=ph1(1,nz,ne,nint)
        //      endif
        //   endif
        if ((nz == 15 || nz == 17 || nz == 19) ||
            (nz > 20 && nz != 26))
            einn = 0;
        else
        {
            if (ne < 3)
                einn = 1E30;
            else
            {
                einn = m_table1[{nz, ne, nint}][0];
            }
        }

        //if(is.lt.nout.and.is.gt.nint.and.e.lt.einn)return
        if (is < nout && is > nint && e < einn)
            return 0;

        //if(is.le.nint.or.e.ge.einn)then
        // p1=-ph1(5,nz,ne,is)
        // y=e/ph1(2,nz,ne,is)
        // q=-0.5*p1-l(is)-5.5
        // a=ph1(3,nz,ne,is)*((y-1.0)**2+ph1(6,nz,ne,is)**2)
        // b=sqrt(y/ph1(4,nz,ne,is))+1.0
        // s=a*y**q*b**p1
        if (is <= nint || e >= einn)
        {
            p1 = -it_Table1->second[4];
            y = e / it_Table1->second[1];
            q = -0.5 * p1 - l[is - 1] - 5.5;
            a = it_Table1->second[2] * ((y - 1) * (y - 1) + sqr(it_Table1->second[5]));
            b = std::pow(y / it_Table1->second[3], 0.5) + 1;
            sigma = a * (std::pow(y, q)) * (std::pow(b, p1));
        }
        //else
        // p1=-ph2(4,nz,ne)
        // q=-0.5*p1-5.5
        // x=e/ph2(1,nz,ne)-ph2(6,nz,ne)
        // z=sqrt(x*x+ph2(7,nz,ne)**2)
        // a=ph2(2,nz,ne)*((x-1.0)**2+ph2(5,nz,ne)**2)
        // b=1.0+sqrt(z/ph2(3,nz,ne))
        // s=a*z**q*b**p1
        else
        {
            p1 = -element_table2[5];
            q = -0.5 * p1 - 5.5;
            x = e / element_table2[2] - element_table2[7];
            z = std::pow(x * x + sqr(element_table2[8]), 0.5);
            a = element_table2[3] * ((x - 1.0) * (x - 1.0) + sqr(element_table2[6]));
            b = 1.0 + std::pow(z / element_table2[4], 0.5);
            sigma = a * std::pow(z, q) * std::pow(b, p1);
        }

        return sigma;
    }

    void PVerner::Table1Hnd::formatShellNumber(t_vect &v) const
    {
        for (phys_size i = v.size(); i > 0; --i)
        {
            v[i - 1].first[2] = getShellNumber(v[i - 1].first[2]);
        }
    }

    phys_size PVerner::Table1Hnd::getShellNumber(phys_size is_) const
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

} // namespace physapi
