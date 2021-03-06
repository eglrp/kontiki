//
// Created by hannes on 2018-01-31.
//

#ifndef KONTIKIV2_SENSORS_HELPER_H
#define KONTIKIV2_SENSORS_HELPER_H

// Eigen::Quaternion not exposed, to use Vector4d instead
using PyRelPosePair = std::pair<Eigen::Vector4d, Eigen::Vector3d>;

template<typename Class, typename PyClass>
static void declare_sensors_common(PyClass &cls) {
  cls.def_property("relative_pose", [](Class &self) {
                     const Eigen::Quaterniond &q = self.relative_orientation();
                     Eigen::Vector4d pyq(q.w(), q.x(), q.y(), q.z());
                     return PyRelPosePair(pyq, self.relative_position());
                   },
                   [](Class &self, const PyRelPosePair &pose) {
                     //Eigen::Quaterniond q(pose.first.data());
                     const Eigen::Vector4d &q = pose.first;
                     self.set_relative_orientation(Eigen::Quaterniond(q(0), q(1), q(2), q(3)));
                     self.set_relative_position(pose.second);
                   });

  cls.def_property("relative_orientation_locked", &Class::RelativeOrientationIsLocked, &Class::LockRelativeOrientation);
  cls.def_property("relative_position_locked", &Class::RelativePositionIsLocked, &Class::LockRelativePosition);
  cls.def_property("time_offset_locked", &Class::TimeOffsetIsLocked, &Class::LockTimeOffset);

  cls.def_property("time_offset", &Class::time_offset, &Class::set_time_offset);
  cls.def_property("max_time_offset", &Class::max_time_offset, &Class::set_max_time_offset);

  cls.def("from_trajectory", &Class::FromTrajectory);
  cls.def("to_trajectory", &Class::ToTrajectory);

}


#endif //KONTIKIV2_SENSORS_HELPER_H
