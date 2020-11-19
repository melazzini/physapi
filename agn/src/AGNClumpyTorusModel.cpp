#include "AGNClumpyTorusModel.hpp"

namespace agn
{
    AGNClumpyTorusModel::AGNClumpyTorusModel(const PSimpleTorus &torus,
                                             phys_float N_H,
                                             phys_float N_aver,
                                             phys_float phi)
        : m_torus{torus},
          m_N_H{N_H}, m_N_aver{N_aver}, m_phi{phi}
    {
        // ! the order here is very important

        m_N_H_clouds = calculateN_H_clouds();
        m_n_H = calculate_n_H();
        m_cloudsRadius = calculateCloudsRadius();
        m_N_clouds_tot = calculateNumOfClouds();
        std::cout << "Num of clouds:  " << m_N_clouds_tot << std::endl;
        std::cout << std::endl;
        createClouds();
    }

    phys_float AGNClumpyTorusModel::distanceToBoundary(const PPosition &position, const PVector3D &direction) const
    {
        auto index = cloudIndex(position);

        if (index >= 0)
        {
            PSphere cloud{static_cast<PPosition>(m_clouds[index]), m_cloudsRadius};
            return cloud.distanceToBoundaryFromInside(position, direction);
        }

        return -1;
    }

    phys_float AGNClumpyTorusModel::distanceToEnteringPoint(const PPosition &position,
                                                            const PVector3D &direction) const
    {
        // find cloud
        // if there is a cloud that will be intersect first
        // then get the distance to it from the given position
        // else return -1

        auto index = findCloud(position, direction);

        if (index >= 0)
        {
            PSphere cloud{static_cast<PPosition>(m_clouds[index]), m_cloudsRadius};
            return cloud.distanceToBoundaryFromOutside(position, direction);
        }

        return -1;
    }

    phys_bool AGNClumpyTorusModel::isInside(const PPosition &position) const
    {
        if (cloudIndex(position) >= 0)
        {
            return true;
        }

        return false;
    }

    void AGNClumpyTorusModel::placeCloud(PVector3D &cloud)
    {
        do
        {
            // we place each cloud inside a cub:
            cloud.setX(m_rndMng.uniform_real(-(m_torus.externalRadius() - m_cloudsRadius),
                                             (m_torus.externalRadius() - m_cloudsRadius))); // length

            cloud.setY(m_rndMng.uniform_real(-(m_torus.externalRadius() - m_cloudsRadius),
                                             (m_torus.externalRadius() - m_cloudsRadius))); // width

            cloud.setZ(m_rndMng.uniform_real(-(m_torus.externalRadius() - m_cloudsRadius),
                                             (m_torus.externalRadius() - m_cloudsRadius))); // height

            phys_float phi_h{std::abs(cloud.phi())};
            phys_float max_phi_h{Pi / 2 - (2 * std::asin(m_cloudsRadius / (2 * cloud.norm())) + m_torus.theta0())};

        } while (!(std::abs(cloud.phi()) < Pi / 2 - (2 * std::asin(m_cloudsRadius / (2 * cloud.norm())) + m_torus.theta0())) or
                 !checkRangeInclusive(m_torus.internalRadius() + m_cloudsRadius,
                                      m_torus.externalRadius() - m_cloudsRadius,
                                      cloud.norm()));
    }

    void AGNClumpyTorusModel::createClouds()
    {
        m_clouds.reserve(m_N_clouds_tot);

        PVector3D cloud_i_pos;

        std::vector<phys_float> d;

        for (phys_size i = 0; i < m_N_clouds_tot; i++)
        {
            if (i % phys_size(0.01 * m_N_clouds_tot) == 0)
            {
                std::cout << "Clouds created: "
                          << 100.0 * phys_float(i) / m_N_clouds_tot
                          << "%       \r";
                std::cout << std::flush;
            }

            do
            {
                placeCloud(cloud_i_pos);

                d.clear();

                for (phys_size j = 0; j < i; j++)
                {
                    d.push_back((cloud_i_pos - m_clouds[j]).norm());
                }

            } while (!noIntersection(d));

            m_clouds.push_back(cloud_i_pos);

            // reset
            cloud_i_pos = PVector3D();
        }
    }

    phys_bool AGNClumpyTorusModel::noIntersection(const std::vector<phys_float> &d) const
    {
        if (d.size() == 0)
        {
            return true;
        }

        for (phys_size i = 0; i < d.size(); i++)
        {
            if (d[i] < (2 * m_cloudsRadius))
            {
                return false;
            }
        }

        return true;
    }

    phys_int AGNClumpyTorusModel::findCloud(const PPosition &pos, const PVector3D &direction) const
    {
        // we initialize the index to = -1
        //it means for us that the photon
        //will not intersect any cloud
        phys_int index{-1};

        //this is the vector from the photon to the cloud
        PVector3D r_photonToCloud{};

        //this is the angle between
        //the photon direction(Omega) and
        //r_photonToCloud
        phys_float Theta{};

        //distance to the photon trajectory
        phys_float d{};

        for (phys_size i = 0; i < m_clouds.size(); i++)
        {
            //initially we get the vector from the photon
            //to the cloud: r_cloud - r_photon
            r_photonToCloud = m_clouds[i] - pos;

            //we also need to know the angle between
            //Omega & r_photonToCloud
            Theta = PVector3D::angleBetweenVectors(direction, r_photonToCloud);

            //if the cloud is "behind" the photon we discard it
            if (Theta >= (Pi / 2.0))
            {
                continue;
            }

            //the distance to the trajectory is the hypotenuse
            d = r_photonToCloud.norm() * std::sin(Theta);

            //if the distance from the cloud to the trajectory
            //is not less than its radius we discard this cloud
            if (d >= m_cloudsRadius)
            {
                continue;
            }

            //if this is the first cloud that could be
            //intersected by the photon then we set
            //its index, and that cloud will be a
            //"candidate"
            if (index < 0)
            {
                index = i;
                continue;
            }

            //if we already have a previous candidate
            //then we compare which cloud is closer
            //to the photon, if that previous one
            //or the current one
            if ((m_clouds[index] - pos).norm() >
                r_photonToCloud.norm())
            {
                //only if the current cloud is closer
                //we update the index of the best candidate
                index = i;
            }
        }

        //finally we return the index of the best
        //candidate or we return -1, which means
        //that there is no cloud that will be
        //intersected by the photon on its
        //current trajectory
        return index;
    }

    phys_int AGNClumpyTorusModel::cloudIndex(const PPosition &position) const
    {
        for (phys_size i = 0; i < m_clouds.size(); i++)
        {
            PSphere cloud{static_cast<PPosition>(m_clouds[i]), m_cloudsRadius};
            if (cloud.isInside(position))
            {
                return i;
            }
        }
        return -1;
    }

} // namespace agn
