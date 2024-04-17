public class Member {
    private String name;
    private String studentId;
    private int age;
    private int grade;
    private double gpa;

    public Member(String name, String studentId, int age, int grade, double gpa) {
        this.name = name;
        this.studentId = studentId;
        this.age = age;
        this.grade = grade;
        this.gpa = gpa;
    }

    @Override
    public String toString() {
        return "Name: " + name + ", Student ID: " + studentId + ", Age: " + age + ", Grade: " + grade + ", GPA: " + gpa;
    }

    // Getters for each field
    public String getName() {
        return name;
    }

    public String getStudentId() {
        return studentId;
    }

    public int getAge() {
        return age;
    }

    public int getGrade() {
        return grade;
    }

    public double getGpa() {
        return gpa;
    }
}