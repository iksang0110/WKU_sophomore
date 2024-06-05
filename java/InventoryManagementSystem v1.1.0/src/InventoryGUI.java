import java.util.Scanner;
import java.io.IOException;

public class InventoryGUI {
    private InventoryManager manager;
    private Scanner scanner;
    private User loggedInUser;

    public InventoryGUI() {
        manager = new InventoryManager();
        scanner = new Scanner(System.in);
    }

    public void displayMainMenu() {
        while (true) {
            System.out.println("┌────────────────────────────┐");
            System.out.println("│      재고 관리 시스템      │");
            System.out.println("├────────────────────────────┤");
            System.out.println("│  1. 상품 추가              │");
            System.out.println("│  2. 사용자 추가            │");
            System.out.println("│  3. 재고 변경              │");
            System.out.println("│  4. 현재 모든 상품 보기    │");
            System.out.println("│  5. 로그아웃               │");
            System.out.println("│  6. 종료                  │");
            System.out.println("└────────────────────────────┘");
            System.out.print("원하는 작업을 선택하세요: ");

            int choice = scanner.nextInt();
            scanner.nextLine();  // 개행 문자 처리

            switch (choice) {
                case 1:
                    if (loggedInUser != null && loggedInUser.getRole().equals("관리자")) {
                        addProduct();
                    } else {
                        System.out.println("권한이 없습니다.");
                    }
                    break;
                case 2:
                    if (loggedInUser != null && loggedInUser.getRole().equals("관리자")) {
                        addUser();
                    } else {
                        System.out.println("권한이 없습니다.");
                    }
                    break;
                case 3:
                    if (loggedInUser != null && (loggedInUser.getRole().equals("관리자") || loggedInUser.getRole().equals("직원"))) {
                        updateProductQuantity();
                    } else {
                        System.out.println("권한이 없습니다.");
                    }
                    break;
                case 4:
                    manager.displayAllProducts();
                    break;
                case 5:
                    logout();
                    return;
                case 6:
                    System.out.println("\n프로그램을 종료합니다. 감사합니다!");
                    return;
                default:
                    System.out.println("\n잘못된 선택입니다. 다시 시도해주세요.");
            }
        }
    }

    public void login() {
        while (true) {
            System.out.println(" _    _  _   __ _   _     ___                                              _              _   ");
            System.out.println("| |  | || | / /| | | |   |_  |                                            (_)            | |  ");
            System.out.println("| |  | || |/ / | | | |     | |  __ _ __   __  __ _   _ __   _ __   ___     _   ___   ___ | |_ ");
            System.out.println("| |/\\| ||    \\ | | | |     | | / _` |\\ \\ / / / _` | | '_ \\ | '__| / _ \\   | | / _ \\ / __|| __|");
            System.out.println("\\  /\\  /| |\\  \\| |_| | /\\__/ /| (_| | \\ V / | (_| | | |_) || |   | (_) |  | ||  __/| (__ | |_ ");
            System.out.println(" \\/  \\/ \\_| \\_/ \\___/  \\____/  \\__,_|  \\_/   \\__,_| | .__/ |_|    \\___/   | | \\___| \\___| \\__|");
            System.out.println("                                                    | |                  _/ |                 ");
            System.out.println("                                                    |_|                 |__/                  ");
            System.out.println("┌────────────────────────────┐");
            System.out.println("│        사용자 로그인       │");
            System.out.println("├────────────────────────────┤");
            System.out.print("│ 사용자 ID: ");
            int userID = scanner.nextInt();
            scanner.nextLine();  // 개행 문자 처리

            System.out.print("│ 비밀번호: ");
            String password = scanner.nextLine();

            loggedInUser = manager.authenticateUser(userID, password);
            if (loggedInUser != null) {
                System.out.println("└────────────────────────────┘");
                System.out.println("로그인 성공! 환영합니다, " + loggedInUser.getName() + "님.");
                break;
            } else {
                System.out.println("└────────────────────────────┘");
                System.out.println("로그인 실패. 다시 시도하세요.");
            }
        }
    }

    private void logout() {
        loggedInUser = null;
        System.out.println("로그아웃 되었습니다.");
        login();
        displayMainMenu();
    }

    private void addProduct() {
        System.out.println("┌────────────────────────────┐");
        System.out.println("│        상품 추가           │");
        System.out.println("├────────────────────────────┤");
        System.out.print("│ 상품 ID: ");
        int productID = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 상품명: ");
        String name = scanner.nextLine();

        System.out.print("│ 가격: ");
        double price = scanner.nextDouble();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 수량: ");
        int quantity = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 카테고리: ");
        String category = scanner.nextLine();

        Product product = new Product(productID, name, price, quantity, category);
        manager.addProduct(product);
        System.out.println("└────────────────────────────┘");
        System.out.println("상품이 추가되었습니다.");
    }

    private void addUser() {
        System.out.println("┌────────────────────────────┐");
        System.out.println("│        사용자 추가         │");
        System.out.println("├────────────────────────────┤");
        System.out.print("│ 사용자 ID: ");
        int userID = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 사용자명: ");
        String name = scanner.nextLine();

        System.out.print("│ 역할: ");
        String role = scanner.nextLine();

        System.out.print("│ 비밀번호: ");
        String password = scanner.nextLine();

        User user = new User(userID, name, role, password);
        manager.addUser(user);
        System.out.println("└────────────────────────────┘");
        System.out.println("사용자가 추가되었습니다.");
    }

    private void updateProductQuantity() {
        System.out.println("┌────────────────────────────┐");
        System.out.println("│        재고 변경           │");
        System.out.println("├────────────────────────────┤");
        System.out.print("│ 상품 ID: ");
        int productID = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        System.out.print("│ 변경 유형 (추가/제거): ");
        String changeType = scanner.nextLine();

        System.out.print("│ 변경 수량: ");
        int quantityChange = scanner.nextInt();
        scanner.nextLine();  // 개행 문자 처리

        boolean isAdding = changeType.equals("추가");
        manager.updateProductQuantity(productID, quantityChange, isAdding);
        System.out.println("└────────────────────────────┘");
        System.out.println("재고가 변경되었습니다.");
    }

    public static void main(String[] args) {
        InventoryGUI gui = new InventoryGUI();
        gui.login();
        gui.displayMainMenu();
    }
}
