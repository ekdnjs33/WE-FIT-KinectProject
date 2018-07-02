package com.hanium.menti.service;

import com.hanium.menti.controller.ReplyJoint;
import com.hanium.menti.entity.Joint;
import com.hanium.menti.repository.JointRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.transaction.Transactional;
import java.util.List;
import java.util.stream.Collectors;

@Service
@Transactional
public class KinectService {

    @Autowired
    private JointRepository jointRepository;

    public Long save(String userId, int jointType, int pointX, int pointY, String timestamp) {
        Joint joint = new Joint(userId, jointType, pointX, pointY, timestamp);
        return jointRepository.save(joint).getId();
    }

    public List<ReplyJoint> findAll() {
        List<Joint> joints = jointRepository.findAll();

        return joints.stream()
                .map(joint -> new ReplyJoint(joint.getId(), joint.getUserId(), joint.getJointType(), joint.getPointX(), joint.getPointY(), joint.getTimestamp()))
                .collect(Collectors.toList());
    }

    public List<ReplyJoint> findOne(String userId) {
        List<Joint> joints = jointRepository.findByUserId(userId);

        return joints.stream()
                .map(joint -> new ReplyJoint(joint.getId(), joint.getUserId(), joint.getJointType(), joint.getPointX(), joint.getPointY(), joint.getTimestamp()))
                .collect(Collectors.toList());
    }
}
