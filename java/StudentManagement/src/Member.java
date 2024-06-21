package WKU_sophomore.java.StudentManagement.src;

import WKU_sophomore.java.StudentManagement.src.MemberManager;
import WKU_sophomore.java.StudentManagement.src.Member;

public class Member { 
    private String name;
    private String regName;
    private int grade;
    private float avePoint;

    public Member(String name, String regName, int grade, float avePoint) {
        this.name = name;
        this.regName = regName;
        this.grade = grade;
        this.avePoint = avePoint;
    }

    public Member() {
        this.name = "";
        this.regName = "";
        this.grade = 0;
        this.avePoint = 0.0f;
    }


    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getRegName() {
        return regName;
    }

    public void setRegName(String regName) {
        this.regName = regName;
    }

    public int getGrade() {
        return grade;
    }

    public void setGrade(int grade) {
        this.grade = grade;
    }

    public float getAvePoint() {
        return avePoint;
    }

    public void setAvePoint(float avePoint) {
        this.avePoint = avePoint;
    }

    @Override
    public String toString() {
        return "Name: " + name + ", Registration Name: " + regName + ", Grade: " + grade + ", Average Point: " + avePoint;
    }
}
