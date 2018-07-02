package com.hanium.menti.repository;

import com.hanium.menti.entity.Joint;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface JointRepository extends JpaRepository<Joint, Long> {
    public List<Joint> findByUserId(String userId);
}
