package com.hanium.menti.controller;

import com.hanium.menti.service.KinectService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/joints")
public class KinectController {

    @Autowired
    private KinectService kinectService;

    @PostMapping("")
    public Long saveData(
            @RequestParam String userId,
            @RequestParam int jointType,
            @RequestParam int pointX,
            @RequestParam int pointY,
            @RequestParam String timestamp){

        return kinectService.save(userId, jointType, pointX, pointY, timestamp);
    }

    @GetMapping("")
    public List<ReplyJoint> findAll() {
        return kinectService.findAll();
    }

    @GetMapping("/user/{userId}")
    public List<ReplyJoint> findOne(@PathVariable String userId) {
        return kinectService.findOne(userId);
    }


}
