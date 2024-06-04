import java.io.*;
import java.util.*;

public class InventoryManager {
    private List<Product> products;
    private List<User> users;
    private String logFile = "inventory_log.txt";

    public InventoryManager() {
        products = new ArrayList<>();
        users = new ArrayList<>();
    }

    // 상품 추가 메서드
    public void addProduct(Product product) {
        products.add(product);
        log("상품 추가: " + product.getName());
    }

    // 사용자 추가 메서드
    public void addUser(User user) {
        users.add(user);
        log("사용자 추가: " + user.getName());
    }

    // 재고 입출고 관리 메서드
    public void updateProductQuantity(int productID, int quantityChange) {
        for (Product product : products) {
            if (product.getProductID() == productID) {
                product.setQuantity(product.getQuantity() + quantityChange);
                log("재고 변경: " + product.getName() + ", 변경 수량: " + quantityChange);
                return;
            }
        }
        log("상품 ID를 찾을 수 없음: " + productID);
    }

    // 파일 입출력 메서드
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

    public void saveProductsToFile(String fileName) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(fileName));
        for (Product product : products) {
            bw.write(product.getProductID() + "," +
                     product.getName() + "," +
                     product.getPrice() + "," +
                     product.getQuantity() + "," +
                     product.getCategory() + "\n");
        }
        bw.close();
    }

    // 로그 기록 메서드
    private void log(String message) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(logFile, true))) {
            bw.write(new Date() + ": " + message + "\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
