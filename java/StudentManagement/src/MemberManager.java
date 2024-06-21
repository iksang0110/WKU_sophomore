package WKU_sophomore.java.StudentManagement.src;


import WKU_sophomore.java.StudentManagement.src.MemberManager;
import WKU_sophomore.java.StudentManagement.src.Member;
import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class MemberManager {
    private List<Member> members = new ArrayList<>();
    private final String fileName = "members.txt";

    public MemberManager() {
        loadData();
    }

    public void addMember(Member member) {
        members.add(member);
    }

    public void viewAllMembers() {
        for (Member member : members) {
            System.out.println(member);
        }
    }

    public Member searchMemberById(int id) {
        for (Member member : members) {
            if (member.getRegName().equals(id)) {
                return member;
            }
        }
        return null;
    }

    private void loadData() {
        try (BufferedReader reader = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] parts = line.split(",");
                String name = parts[0];
                String regName = parts[1];
                int grade = Integer.parseInt(parts[2]);
                float avePoint = Float.parseFloat(parts[3]);
                members.add(new Member(name, regName, grade, avePoint));
            }
        } catch (IOException e) {
            System.out.println("Error loading data: " + e.getMessage());
        }
    }

    public void saveData() {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName))) {
            for (Member member : members) {
                writer.write(member.getName() + "," + member.getRegName() + "," + member.getGrade() + "," + member.getAvePoint());
                writer.newLine();
            }
        } catch (IOException e) {
            System.out.println("Error saving data: " + e.getMessage());
        }
    }
}
