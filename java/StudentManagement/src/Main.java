package WKU_sophomore.java.StudentManagement.src;


import WKU_sophomore.java.StudentManagement.src.MemberManager;
import WKU_sophomore.java.StudentManagement.src.Member;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        MemberManager manager = new MemberManager();
        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println("1. 등록 학생 리스트 보기");
            System.out.println("2. 등록 학생 학번 조회");
            System.out.println("3. 새로운 학생 정보 등록");
            System.out.println("4. 종료");
            System.out.print("선택: ");
            int choice = scanner.nextInt();

            switch (choice) {
                case 1:
                    manager.viewAllMembers();
                    break;
                case 2:
                    System.out.print("학번 입력: ");
                    String regName = scanner.next();
                    Member member = manager.searchMemberById(regName);
                    if (member != null) {
                        System.out.println(member);
                    } else {
                        System.out.println("해당 학번의 학생이 없습니다.");
                    }
                    break;
                case 3:
                    System.out.print("학번 입력: ");
                    String newRegName = scanner.next();
                    scanner.nextLine();  
                    System.out.print("이름 입력: ");
                    String name = scanner.nextLine();
                    System.out.print("학년 입력: ");
                    int grade = scanner.nextInt();
                    System.out.print("평균 학점 입력: ");
                    float avePoint = scanner.nextFloat();
                    manager.addMember(new member(name, newRegName, grade, avePoint));
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
