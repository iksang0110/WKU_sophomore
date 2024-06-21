프로그램 조건
1. 목적: 학생들의 평균 학점 정보를 관리하며, 필요할 때 학생들의 평균 학점 정보를 조회하기 위한 목적임
2. 기능 
a. 등록된 학생들의 전체 정보 보기
b. 등록된 학생들 정보 학번으로 조회
c. 새로운 학생 정보 입력
3. 조건
a. 데이터 모델 제시 (파일 Data structure 제시) 주석으로 작성
b. 제시된 데이터 모델 반영 클래스 모델 제시
c. 각 데이터에 접근할 수 있는 getter/setter 구현
d. 관리되는 학생 정보는 모두 파일에 저장
e. 프로그램 시작 시 파일에 저장된 데이터 로드, 새로운 학생 정보 추가 또는 프로그램 종료시 데이터 저장 기능 클래스로 정의
import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Student {
    private int id;
    private String name;
    private double averageGrade;

    public Student(int id, String name, double averageGrade) {
        this.id = id;
        this.name = name;
        this.averageGrade = averageGrade;
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public double getAverageGrade() {
        return averageGrade;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setAverageGrade(double averageGrade) {
        this.averageGrade = averageGrade;
    }

    @Override
    public String toString() {
        return "ID: " + id + ", Name: " + name + ", Average Grade: " + averageGrade;
    }
}

class StudentManager {
    private List<Student> students = new ArrayList<>();
    private final String fileName = "students.txt";

    public StudentManager() {
        loadData();
    }

    public void addStudent(Student student) {
        students.add(student);
    }

    public void viewAllStudents() {
        for (Student student : students) {
            System.out.println(student);
        }
    }

    public Student searchStudentById(int id) {
        for (Student student : students) {
            if (student.getId() == id) {
                return student;
            }
        }
        return null;
    }

    private void loadData() {
        try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");
                int id = Integer.parseInt(parts[0]);
                String name = parts[1];
                double averageGrade = Double.parseDouble(parts[2]);
                students.add(new Student(id, name, averageGrade));
            }
        } catch (IOException e) {
            System.out.println("Error loading data: " + e.getMessage());
        }
    }

    public void saveData() {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName))) {
            for (Student student : students) {
                writer.write(student.getId() + "," + student.getName() + "," + student.getAverageGrade());
                writer.newLine();
            }
        } catch (IOException e) {
            System.out.println("Error saving data: " + e.getMessage());
        }
    }
}

public class Main {
    public static void main(String[] args) {
        StudentManager manager = new StudentManager();
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("1. 모든 학생 정보 보기");
            System.out.println("2. 학번으로 학생 정보 조회");
            System.out.println("3. 새로운 학생 정보 추가");
            System.out.println("4. 종료");
            System.out.print("선택: ");
            int choice = scanner.nextInt();

            switch (choice) {
                case 1:
                    manager.viewAllStudents();
                    break;
                case 2:
                    System.out.print("학번 입력: ");
                    int id = scanner.nextInt();
                    Student student = manager.searchStudentById(id);
                    if (student != null) {
                        System.out.println(student);
                    } else {
                        System.out.println("해당 학번의 학생이 없습니다.");
                    }
                    break;
                case 3:
                    System.out.print("학번 입력: ");
                    int newId = scanner.nextInt();
                    scanner.nextLine();  // consume newline
                    System.out.print("이름 입력: ");
                    String name = scanner.nextLine();
                    System.out.print("평균 학점 입력: ");
                    double averageGrade = scanner.nextDouble();
                    manager.addStudent(new Student(newId, name, averageGrade));
                    manager.saveData();
                    break;
                case 4:
                    manager.saveData();
                    System.out.println("프로그램을 종료합니다.");
                    return;
                default:
                    System.out.println("잘못된 선택입니다.");
            }
        }
    }
}
