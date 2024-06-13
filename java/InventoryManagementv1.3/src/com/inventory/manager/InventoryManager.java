package com.inventory.manager;

import com.inventory.model.Product;
import com.inventory.model.User;
import com.inventory.model.UserInterface;

import java.io.*;
import java.util.ArrayList;
import java.util.Date; // 추가된 부분
import java.util.List;

public class InventoryManager {
    private List<Product> products;
    private List<UserInterface> users;
    private String logFile = "inventory_log.txt";
    private String productFile = "products.txt";

    public InventoryManager() {
        products = new ArrayList<>();
        users = new ArrayList<>();
        // 관리자 추가
        users.add(new User(1234, "20212316 정익상", "관리자", "0000"));
        // 프로그램 시작 시 파일에서 데이터 로드
        try {
            loadProductsFromFile(productFile);
        } catch (IOException e) {
            System.out.println("파일 로드 중 오류가 발생했습니다: " + e.getMessage());
        }
    }

    public void addProduct(Product product) {
        products.add(product);
        log("상품 추가: " + product.getName());
        saveProductsToFile();
    }

    public void addUser(User user) {
        users.add(user);
        log("사용자 추가: " + user.getName());
        saveProductsToFile();
    }

    public void updateProductQuantity(int productID, int quantityChange, boolean isAdding) {
        for (Product product : products) {
            if (product.getProductID() == productID) {
                if (isAdding) {
                    product.setQuantity(product.getQuantity() + quantityChange);
                    log("재고 추가: " + product.getName() + ", 추가 수량: " + quantityChange);
                } else {
                    product.setQuantity(product.getQuantity() - quantityChange);
                    log("재고 감소: " + product.getName() + ", 감소 수량: " + quantityChange);
                }
                saveProductsToFile();
                return;
            }
        }
        log("상품 ID를 찾을 수 없음: " + productID);
    }

    public void loadProductsFromFile(String fileName) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(fileName));
        String line;
        while ((line = br.readLine()) != null) {
            String[] data = line.split(",");
            Product product = new Product(
                    Integer.parseInt(data[0]),
                    data[1],
                    Double.parseDouble(data[2]),
                    Integer.parseInt(data[3]),
                    data[4]
            );
            products.add(product);
        }
        br.close();
    }

    public void saveProductsToFile() {
        try {
            BufferedWriter bw = new BufferedWriter(new FileWriter("products.txt"));
            for (Product product : products) {
                bw.write(product.getProductID() + "," +
                         product.getName() + "," +
                         product.getPrice() + "," +
                         product.getQuantity() + "," +
                         product.getCategory() + "\n");
            }
            bw.close();
        } catch (IOException e) {
            System.out.println("파일 저장 중 오류가 발생했습니다: " + e.getMessage());
        }
    }

    private void log(String message) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(logFile, true))) {
            bw.write(new Date() + ": " + message + "\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void displayAllProducts() {
        System.out.println("┌────────────────────────────┐");
        System.out.println("│      현재 모든 상품        │");
        System.out.println("├────────────────────────────┤");
        for (Product product : products) {
            System.out.println("│ 상품 ID: " + product.getProductID());
            System.out.println("│ 상품명: " + product.getName());
            System.out.println("│ 가격: " + product.getPrice());
            System.out.println("│ 수량: " + product.getQuantity());
            System.out.println("│ 카테고리: " + product.getCategory());
            System.out.println("├────────────────────────────┤");
        }
        System.out.println("└────────────────────────────┘");
    }

    public UserInterface authenticateUser(int userID, String password) {
        for (UserInterface user : users) {
            if (user.getUserID() == userID && user.getPassword().equals(password)) {
                return user;
            }
        }
        return null;
    }

    public List<Product> getProducts() {
        return products;
    }
}
