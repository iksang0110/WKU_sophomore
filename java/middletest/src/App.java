import java.util.Scanner;

public class App {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        // 1.정수를 입력 받아 출력하기  
        System.out.println("1번: 정수를 입력하세요.");
        System.out.println("입력한 정수: "+scanner.nextInt());

        // 2.두 정수를 입력 받아 출력하기  
        System.out.println("2번: 두 정수를 입력하세요.");
        System.out.println("입력한 첫 번째 정수: " +scanner.nextInt()+", 두 번째 정수: "+scanner.nextInt());

        //3.두 정수를 입력 받아 변수에 저장하고 출력하기
        System.out.println("3번: 두 정수를 입력하세요.");
        int num1 = scanner.nextInt();
        int num2 = scanner.nextInt();
        System.out.println("저장된 첫 번쨰 정수:"+num1+"두 번째 정수:"+num2);

        //4.두 정수를 입력 받아 한 번에 출력하기.
        System.out.println("4번: 두 정수를 입력하세요.");
        System.out.println("입력된 두 정수: " +scanner.nextInt()+ " "+scanner.nextInt());

        //5.두 정수를 입력 받아 줄바꿈 없이 출력하기
        System.out.println("5번: 두 정수를 입력하세요.");
        System.out.print("줄바꿈 없이 출력:"+scanner.nextInt()+" "+scanner.nextInt()+"\n");

        //6.두 정수를 입력 받아 변수에 저장하고, 저장된 두 변수의 값을 교환 후 출력하기
        System.out.println("6번: 두 정수를 입력하세요");
        int num3 = scanner.nextInt();
        int num4 = scanner.nextInt();
        int temp = num3;
        num3 = num4;
        num4 = temp;
        System.out.println("교환된 두 정수:"+num3+ " "+num4);

         //7. 정수를 입력 받아 비트 왼쪽 시프트 연산하기 (이동 값은 임의 설정, 예: 2)
        System.out.println("7번: 정수를 입력하세요.");
        int num5 = scanner.nextInt();
        System.out.println("비트 왼쪽 시프트 연산 결과 (" + num5 + " << 2): " + (num5 << 2));

         //8. 정수를 입력 받아 비트 오른쪽 시프트 연산하기 (이동 값은 임의 설정, 예: 2)
        System.out.println("8번: 정수를 입력하세요.");
        int num6 = scanner.nextInt();
        System.out.println("비트 오른쪽 시프트 연산 결과 (" + num6 + " >> 2): " + (num6 >> 2));

        scanner.close();
    }
}           