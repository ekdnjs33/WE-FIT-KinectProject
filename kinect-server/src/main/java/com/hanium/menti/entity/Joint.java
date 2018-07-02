package com.hanium.menti.entity;

import org.springframework.format.datetime.standard.InstantFormatter;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

@Entity
public class Joint {

    public Joint(String userId, int jointType, int pointX, int pointY, String timestamp){
        this.userId = userId;
        this.jointType = jointType;
        this.pointX = pointX;
        this.pointY = pointY;
        this.timestamp = LocalDateTime.parse(timestamp, DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss[.nnn]"));
    }

    public Joint(){}

    @Id
    @GeneratedValue
    private Long id;

    private String userId;
    private int jointType;
    private int pointX;
    private int pointY;

    private LocalDateTime timestamp;

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

    public LocalDateTime getTimestamp() {
        return timestamp;
    }
}
