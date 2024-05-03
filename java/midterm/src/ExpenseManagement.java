//프로그램 조건 
//a. 용돈 사용 내역을 저장하고, 검색하는 프로그램을 작성하려한다.
//b. 사용자로부터 "사용목적", "사용금액","사용일"을 입력받는다.
//c. "전체 사용내역 보기", "사용내역 추가"기능을 갖는다.
//d. 클래스에 필드를 모두 private로 선언하고, getter와 setter를 제공한다. 

import java.util.ArrayList;
import java.util.Scanner;

// 지출 내역을 관리하는 클래스
class Expense {
    // 지출 내역의 비공개 필드
    private String purpose; // 지출 목적
    private int amount;     // 지출 금액
    private String date;    // 지출 날짜

    // 클래스 생성자
    public Expense(String purpose, int amount, String date) {
        this.purpose = purpose;
        this.amount = amount;
        this.date = date;
    }

    // 목적에 대한 getter와 setter
    public String getPurpose() {
        return purpose;
    }

    public void setPurpose(String purpose) {
        this.purpose = purpose;
    }

    // 금액에 대한 getter와 setter
    public int getAmount() {
        return amount;
    }

    public void setAmount(int amount) {
        this.amount = amount;
    }

    // 날짜에 대한 getter와 setter
    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    // 지출 내역을 출력하는 메소드
    public String displayExpense() {
        return "목적: " + purpose + ", 금액: " + amount + ", 날짜: " + date;
    }
}

//클래스
public class ExpenseManagement {
    private static ArrayList<Expense> expenses = new ArrayList<>(); // 지출 내역을 저장하는 리스트
    private static Scanner scanner = new Scanner(System.in); // 입력을 위한 Scanner 객체

    // 새 지출 내역을 추가하는 메소드
    public static void addExpense() {
        System.out.println("새 지출 내역을 입력하세요.");
        System.out.print("사용 목적: ");
        String purpose = scanner.nextLine();
        System.out.print("사용 금액 (원): ");
        int amount = Integer.parseInt(scanner.nextLine());
        System.out.print("사용 날짜 (YYYY.MM.DD): ");
        String date = scanner.nextLine();

        Expense newExpense = new Expense(purpose, amount, date);
        expenses.add(newExpense);
        System.out.println("새로운 지출 내역이 추가되었습니다.");
    }

    // 저장된 지출 내역을 조회하는 메소드
    public static void viewExpenses() {
        if (expenses.isEmpty()) {
            System.out.println("지출 내역이 없습니다.");
        } else {
            System.out.println("전체 지출 내역:");
            for (Expense expense : expenses) {
                System.out.println(expense.displayExpense());
            }
        }
    }

    // 메인 메소드2
    public static void main(String[] args) {
        while (true) {
            System.out.println("\n1. 전체 사용내역 보기\n2. 사용내역 추가\n3. 종료");
            System.out.print("선택: ");
            String choice = scanner.nextLine();
            switch (choice) {
                case "1":
                    viewExpenses();
                    break;
                case "2":
                    addExpense();
                    break;
                case "3":
                    System.out.println("프로그램을 종료합니다.");
                    scanner.close(); // Scanner 객체를 닫아 리소스 해제
                    return;
                default:
                    System.out.println("잘못된 선택입니다. 다시 입력해 주세요.");
                    break;
            }
        }
    }
}