package com.hanium.menti.controller;

import java.time.LocalDateTime;

public class ReplyJoint {
    private Long id;
    private String userId;
    private int jointType;
    private int pointX;
    private int pointY;
    private LocalDateTime timestamp;

    public ReplyJoint(Long id, String userId, int jointType, int pointX, int pointY, LocalDateTime timestamp) {
        this.id = id;
        this.userId = userId;
        this.jointType = jointType;
        this.pointX = pointX;
        this.pointY = pointY;
        this.timestamp = timestamp;
    }

    public Long getId() {
        return id;
    }

    public String getUserId() {
        return userId;
    }

    public int getJointType() {
        return jointType;
    }

    public int getPointX() {
        return pointX;
    }

    public int getPointY() {
        return pointY;
    }

    public LocalDateTime getTimestamp(){
        return timestamp;
    }
}
