import java.util.Scanner;

public class app2 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("점수를 입력하세요(0~100): ");
        int score = scanner.nextInt();

        if (score < 0 || score > 100) {
            System.out.println("에러: 입력 범위를 벗어났습니다.");
        } else if (score >= 80) {
            System.out.println("합격");
        } else if (score >= 60) {
            System.out.println("재시험");
        } else {
            System.out.println("불합격");
        }
    }
}