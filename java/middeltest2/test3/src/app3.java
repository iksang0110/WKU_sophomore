import java.util.Scanner;

public class app3{
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("세 정수를 입력하세요: ");
        int a = scanner.nextInt();
        int b = scanner.nextInt();
        int c = scanner.nextInt();

        int max = Math.max(a, Math.max(b, c));
        int min = Math.min(a, Math.min(b, c));
        int sum = a + b + c;
        double avg = sum / 3.0;

        System.out.println("최대값: " + max);
        System.out.println("최소값: " + min);
        System.out.println("합계: " + sum);
        System.out.println("평균: " + avg);
    }
}