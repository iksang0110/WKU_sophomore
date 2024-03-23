// import java.util.Scanner;

// public class App {
//     public static void main(String[] args) {
//         Scanner scanner = new Scanner(System.in);
        
//         // 1.정수를 입력 받아 출력하기  
//         System.out.println("1번: 정수를 입력하세요.");
//         System.out.println("입력한 정수: "+scanner.nextInt());

//         // 2.두 정수를 입력 받아 출력하기  
//         System.out.println("2번: 두 정수를 입력하세요.");
//         System.out.println("입력한 첫 번째 정수: " +scanner.nextInt()+", 두 번째 정수: "+scanner.nextInt());

//         //3.두 정수를 입력 받아 변수에 저장하고 출력하기
//         System.out.println("3번: 두 정수를 입력하세요.");
//         int num1 = scanner.nextInt();
//         int num2 = scanner.nextInt();
//         System.out.println("저장된 첫 번쨰 정수:"+num1+"두 번째 정수:"+num2);

//         //4.두 정수를 입력 받아 한 번에 출력하기.
//         System.out.println("4번: 두 정수를 입력하세요.");
//         System.out.println("입력된 두 정수: " +scanner.nextInt()+ " "+scanner.nextInt());

//         //5.두 정수를 입력 받아 줄바꿈 없이 출력하기
//         System.out.println("5번: 두 정수를 입력하세요.");
//         System.out.print("줄바꿈 없이 출력:"+scanner.nextInt()+" "+scanner.nextInt()+"\n");

//         //6.두 정수를 입력 받아 변수에 저장하고, 저장된 두 변수의 값을 교환 후 출력하기
//         System.out.println("6번: 두 정수를 입력하세요");
//         int num3 = scanner.nextInt();
//         int num4 = scanner.nextInt();
//         int temp = num3;
//         num3 = num4;
//         num4 = temp;
//         System.out.println("교환된 두 정수:"+num3+ " "+num4);

//          //7. 정수를 입력 받아 비트 왼쪽 시프트 연산하기 (이동 값은 임의 설정, 예: 2)
//         System.out.println("7번: 정수를 입력하세요.");
//         int num5 = scanner.nextInt();
//         System.out.println("비트 왼쪽 시프트 연산 결과 (" + num5 + " << 2): " + (num5 << 2));

//          //8. 정수를 입력 받아 비트 오른쪽 시프트 연산하기 (이동 값은 임의 설정, 예: 2)
//         System.out.println("8번: 정수를 입력하세요.");
//         int num6 = scanner.nextInt();
//         System.out.println("비트 오른쪽 시프트 연산 결과 (" + num6 + " >> 2): " + (num6 >> 2));

//         scanner.close();
//     }
// }           

import java.util.Scanner;

public class App {
    public static void main(String[] args) throws Exception {
        Scanner stdin = new Scanner(System.in);
        System.out.print("첫 번째 정수를 입력하시오 :");
        int a = Integer.parseInt(stdin.nextLine()); // 입력 값을 정수로 변환
        System.out.println("첫 번째 정수 a의 값 :" + a);

        System.out.print("두 번째 정수를 입력하시요 :");
        int b = Integer.parseInt(stdin.nextLine()); // 입력 값을 정수로 변환
        System.out.print("세 번째 정수를 입력하시요 :");
        int c = Integer.parseInt(stdin.nextLine()); // 입력 값을 정수로 변환

        System.out.println("두 정수 b와 c의 값 : " + b + " " + c);
        System.out.printf("두 정수 b와 c의 값 : %d %d\n", b, c);

        int temp = b; // b 값을 temp에 저장
        b = c; // b에 c 값을 할당
        c = temp; // c에 temp 값을 할당

        System.out.println("swap된 b와 c의 값 : " + b + " " + c);

        System.out.println("a << 2 = " + (a << 2) + " (" + Integer.toBinaryString(a << 2) + ")");
        System.out.println("b << 2 = " + (b << 2) + " (" + Integer.toBinaryString(b << 2) + ")");
        System.out.println("a >> 2 = " + (a >> 2) + " (" + Integer.toBinaryString(a >> 2) + ")");
        System.out.println("b >> 2 = " + (b >> 2) + " (" + Integer.toBinaryString(b >> 2) + ")");
    }
}