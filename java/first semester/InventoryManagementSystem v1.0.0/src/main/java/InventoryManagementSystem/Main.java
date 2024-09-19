package WKU_sophomore.java.InventoryManagementSystem.src.main.java.InventoryManagementSystem;

import java.io.IOException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        InventoryManager manager = new InventoryManager();
        Scanner scanner = new Scanner(System.in);

        try {
            manager.loadProductsFromFile();
            manager.loadUsersFromFile();
        } catch (IOException | ClassNotFoundException e) {
            System.out.println("데이터를 불러오는 중 오류 발생: " + e.getMessage());
        }

        while (true) {
            System.out.println("스마트 재고관리 시스템");
            System.out.println("1. 상품 추가");
            System.out.println("2. 사용자 추가");
            System.out.println("3. 상품 목록 보기");
            System.out.println("4. 사용자 목록 보기");
            System.out.println("5. 종료");
            System.out.print("메뉴를 선택하세요: ");
            int choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice) {
                case 1:
                    System.out.print("상품 ID: ");
                    String productId = scanner.nextLine();
                    System.out.print("상품명: ");
                    String name = scanner.nextLine();
                    System.out.print("가격: ");
                    double price = scanner.nextDouble();
                    System.out.print("수량: ");
                    int quantity = scanner.nextInt();
                    scanner.nextLine();
                    System.out.print("카테고리: ");
                    String category = scanner.nextLine();
                    Product product = new Product(productId, name, price, quantity, category);
                    manager.addProduct(product);
                    break;
                case 2:
                    System.out.print("사용자 ID: ");
                    String userId = scanner.nextLine();
                    System.out.print("사용자명: ");
                    String userName = scanner.nextLine();
                    System.out.print("역할: ");
                    String role = scanner.nextLine();
                    User user = new User(userId, userName, role);
                    manager.addUser(user);
                    break;
                case 3:
                    for (Product p : manager.getProducts()) {
                        System.out.println(p);
                    }
                    break;
                case 4:
                    for (User u : manager.getUsers()) {
                        System.out.println(u);
                    }
                    break;
                case 5:
                    try {
                        manager.saveProductsToFile();
                        manager.saveUsersToFile();
                    } catch (IOException e) {
                        System.out.println("데이터를 저장하는 중 오류 발생: " + e.getMessage());
                    }
                    System.out.println("프로그램을 종료합니다.");
                    return;
                default:
                    System.out.println("잘못된 입력입니다. 다시 시도하세요.");
            }
        }
    }
}
