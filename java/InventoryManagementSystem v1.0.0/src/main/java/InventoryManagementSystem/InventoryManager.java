package WKU_sophomore.java.InventoryManagementSystem.src.main.java.InventoryManagementSystem;

import java.io.*;
import java.util.*;
import InventoryManagementSystem.Product;
import InventoryManagementSystem.User;

public class InventoryManager {
    private List<Product> products = new ArrayList<>();
    private List<User> users = new ArrayList<>();
    private static final String PRODUCT_FILE = "data/products.dat";
    private static final String USER_FILE = "data/users.dat";
    private static final String LOG_FILE = "data/inventory_log.txt";

    public void addProduct(Product product) {
        products.add(product);
        logAction("상품 추가: " + product.getName());
    }

    public void addUser(User user) {
        users.add(user);
    }

    public void saveProductsToFile() throws IOException {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(PRODUCT_FILE))) {
            oos.writeObject(products);
        }
    }

    public void loadProductsFromFile() throws IOException, ClassNotFoundException {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(PRODUCT_FILE))) {
            products = (List<Product>) ois.readObject();
        }
    }

    public void saveUsersToFile() throws IOException {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream(USER_FILE))) {
            oos.writeObject(users);
        }
    }

    public void loadUsersFromFile() throws IOException, ClassNotFoundException {
        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(USER_FILE))) {
            users = (List<User>) ois.readObject();
        }
    }

    private void logAction(String action) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(LOG_FILE, true))) {
            writer.write(new Date() + " - " + action);
            writer.newLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public List<Product> getProducts() {
        return products;
    }

    public List<User> getUsers() {
        return users;
    }
}
