import java.util.Scanner;

public class app4 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("구입할 사과의 개수를 입력하세요: ");
        int count = scanner.nextInt();
        int price;

        if (count < 10) {
            price = 1200;
        } else if (count < 40) {
            price = 1000;
        } else {
            price = 800;
        }

        System.out.println("총 결제 금액: " + (price * count) + "원");
    }
}