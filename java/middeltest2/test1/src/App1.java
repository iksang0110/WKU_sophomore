import java.util.Scanner;

public class App1 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("정수를 입력하세요: ");
        int number = scanner.nextInt();

        if (number % 2 == 0 && number % 3 == 0) {
            System.out.println("2와 3의 배수입니다.");
        } else if (number % 2 == 0) {
            System.out.println("2의 배수입니다.");
        } else if (number % 3 == 0) {
            System.out.println("3의 배수입니다.");
        } else {
            System.out.println("2와 3의 배수가 아닙니다.");
        }
    }
}
