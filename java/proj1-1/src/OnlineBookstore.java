/*

데이터 모델을 위한 클래스 설계:

Customer 클래스: 고객 정보 저장.
Product 클래스: 상품 정보 저장.
CartItem 클래스: 장바구니 항목을 나타냄.
ShoppingCart 클래스: 장바구니 작업을 관리.
Receipt 클래스: 영수증을 생성.
Admin 클래스: 관리자 로그인 처리.

각 클래스 구현:

각 클래스에 필요한 속성과 메서드를 정의.
각 메서드가 필요한 작업을 수행하도록 구현.
메인 프로그램 작성:

기능과 상호작용할 수 있는 메뉴 제공.
프로그램이 올바르게 실행되고 모든 요구사항을 충족하는지 확인.*/


import java.util.ArrayList;
import java.util.Scanner;

// 고객 정보 클래스
class Customer {
    private String name;
    private String email;

    public Customer(String name, String email) {
        this.name = name;
        this.email = email;
    }

    public String getName() {
        return name;
    }

    public String getEmail() {
        return email;
    }
}

// 상품 클래스
class Product {
    private String name;
    private double price;

    public Product(String name, double price) {
        this.name = name;
        this.price = price;
    }

    public String getName() {
        return name;
    }

    public double getPrice() {
        return price;
    }
}

// 장바구니 항목 클래스
class CartItem {
    private Product product;
    private int quantity;

    public CartItem(Product product, int quantity) {
        this.product = product;
        this.quantity = quantity;
    }

    public Product getProduct() {
        return product;
    }

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    public double getTotalPrice() {
        return product.getPrice() * quantity;
    }
}

// 장바구니 클래스
class ShoppingCart {
    private ArrayList<CartItem> items;

    public ShoppingCart() {
        this.items = new ArrayList<>();
    }

    public void addItem(Product product, int quantity) {
        for (CartItem item : items) {
            if (item.getProduct().getName().equals(product.getName())) {
                item.setQuantity(item.getQuantity() + quantity);
                return;
            }
        }
        items.add(new CartItem(product, quantity));
    }

    public void removeItem(String productName) {
        items.removeIf(item -> item.getProduct().getName().equals(productName));
    }

    public void reduceItemQuantity(String productName, int quantity) {
        for (CartItem item : items) {
            if (item.getProduct().getName().equals(productName)) {
                int newQuantity = item.getQuantity() - quantity;
                if (newQuantity > 0) {
                    item.setQuantity(newQuantity);
                } else {
                    removeItem(productName);
                }
                return;
            }
        }
    }

    public void clearCart() {
        items.clear();
    }

    public void showCart() {
        if (items.isEmpty()) {
            System.out.println("장바구니가 비어 있습니다.");
        } else {
            for (CartItem item : items) {
                System.out.println(item.getProduct().getName() + " - 수량: " + item.getQuantity() + " - 총 가격: " + item.getTotalPrice());
            }
        }
    }

    public double getTotalPrice() {
        double total = 0;
        for (CartItem item : items) {
            total += item.getTotalPrice();
        }
        return total;
    }
}

// 영수증 클래스
class Receipt {
    public static void printReceipt(ShoppingCart cart) {
        System.out.println("===== 영수증 =====");
        cart.showCart();
        System.out.println("총 가격: " + cart.getTotalPrice());
        System.out.println("=================");
    }
}

// 관리자 클래스
class Admin {
    private String username;
    private String password;

    public Admin(String username, String password) {
        this.username = username;
        this.password = password;
    }

    public boolean login(String username, String password) {
        return this.username.equals(username) && this.password.equals(password);
    }
}

// 메인 클래스
public class OnlineBookstore {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        ShoppingCart cart = new ShoppingCart();
        Admin admin = new Admin("admin", "password");

        while (true) {
            System.out.println("1. 고객 정보 확인");
            System.out.println("2. 장바구니 상품 목록 보기");
            System.out.println("3. 장바구니 비우기");
            System.out.println("4. 항목 추가하기");
            System.out.println("5. 장바구니 항목 수량 줄이기");
            System.out.println("6. 장바구니 항목 삭제하기");
            System.out.println("7. 영수증 표시하기");
            System.out.println("8. 관리자 로그인");
            System.out.println("9. 종료");
            System.out.print("선택: ");
            int choice = scanner.nextInt();
            scanner.nextLine();  // 개행 문자 제거

            switch (choice) {
                case 1:
                    System.out.print("이름: ");
                    String name = scanner.nextLine();
                    System.out.print("이메일: ");
                    String email = scanner.nextLine();
                    Customer customer = new Customer(name, email);
                    System.out.println("고객 이름: " + customer.getName());
                    System.out.println("고객 이메일: " + customer.getEmail());
                    break;
                case 2:
                    cart.showCart();
                    break;
                case 3:
                    cart.clearCart();
                    System.out.println("장바구니를 비웠습니다.");
                    break;
                case 4:
                    System.out.print("상품 이름: ");
                    String productName = scanner.nextLine();
                    System.out.print("상품 가격: ");
                    double productPrice = scanner.nextDouble();
                    System.out.print("수량: ");
                    int quantity = scanner.nextInt();
                    cart.addItem(new Product(productName, productPrice), quantity);
                    System.out.println("상품을 추가했습니다.");
                    break;
                case 5:
                    System.out.print("상품 이름: ");
                    String reduceProductName = scanner.nextLine();
                    System.out.print("수량 줄이기: ");
                    int reduceQuantity = scanner.nextInt();
                    cart.reduceItemQuantity(reduceProductName, reduceQuantity);
                    System.out.println("상품 수량을 줄였습니다.");
                    break;
                case 6:
                    System.out.print("상품 이름: ");
                    String removeProductName = scanner.nextLine();
                    cart.removeItem(removeProductName);
                    System.out.println("상품을 삭제했습니다.");
                    break;
                case 7:
                    Receipt.printReceipt(cart);
                    break;
                case 8:
                    System.out.print("사용자 이름: ");
                    String username = scanner.nextLine();
                    System.out.print("비밀번호: ");
                    String password = scanner.nextLine();
                    if (admin.login(username, password)) {
                        System.out.println("관리자 로그인 성공.");
                    } else {
                        System.out.println("로그인 실패.");
                    }
                    break;
                case 9:
                    System.out.println("프로그램을 종료합니다.");
                    scanner.close();
                    return;
                default:
                    System.out.println("잘못된 선택입니다.");
                    break;
            }
        }
    }
}

