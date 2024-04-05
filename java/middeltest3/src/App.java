import java.util.Scanner;

public class App {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // 1. 
        while (true) {
            System.out.println("숫자를 입력하세요. (종료하려면 '999'를 입력): ");
            int number = scanner.nextInt();
            if (number == 999) 
            break;
            if (number > 0) {
                System.out.println("양수입니다.");
            } else if (number < 0) {
                System.out.println("음수입니다.");
            } else {
                System.out.println("0입니다.");
            }
        }

        // 2. 
        for (int i = 7; i > 0; i--) {
            for (int j = 0; j < i; j++) {
                System.out.print("*");
            }
            System.out.println();
        }

        // 3. 
        for (int i = 1; i <= 73; i++) {
            if (i % 10 == 3) {
                System.out.println(i);
            }
        }

        // 4. 
        while (true) {
            System.out.println("1. 홀수 단 구구단\n2. 짝수 단 구구단\n3. 종료\n선택: ");
            int choice = scanner.nextInt();
            switch (choice) {
                case 1:
                    for (int i = 3; i <= 9; i += 2) {
                        for (int j = 1; j <= 9; j++) {
                            System.out.println(i + " * " + j + " = " + (i * j));
                        }
                        System.out.println();
                    }
                    break;
                case 2:
                    for (int i = 2; i <= 8; i += 2) {
                        for (int j = 1; j <= 9; j++) {
                            System.out.println(i + " * " + j + " = " + (i * j));
                        }
                        System.out.println();
                    }
                    break;
                case 3:
                    System.out.println("프로그램을 종료합니다.");
                    return;
                default:
                    System.out.println("잘못된 입력입니다. 다시 입력하세요.");
            }
        }
    }
}
