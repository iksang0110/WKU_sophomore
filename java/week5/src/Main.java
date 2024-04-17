import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        List<Member> members = new ArrayList<>();
        members.add(new Member("정익상", "20212316", 23, 2, 4.2));
        members.add(new Member("홍길동", "20210021", 21, 2, 3.7));
        members.add(new Member("아무개", "20210316", 22, 3, 4.0));

        Scanner scanner = new Scanner(System.in);
        System.out.print("찾을 파일을 적으세요 (이름/학번/학년): ");
        String searchField = scanner.nextLine();
        System.out.print("학점은? " + searchField + ": ");
        String searchValue = scanner.nextLine();

        List<Member> results = Utils.searchMembers(members, searchField, searchValue);
        Utils.displayResult(results);

        scanner.close();
    }
}