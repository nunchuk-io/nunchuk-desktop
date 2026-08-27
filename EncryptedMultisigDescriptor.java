package nunchuk.crypto;

import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.SecureRandom;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Base64;
import java.util.List;
import javax.crypto.Cipher;
import javax.crypto.SecretKeySpec;
import javax.crypto.spec.GCMParameterSpec;
import javax.crypto.AEADBadTagException;

/**
 * Provides 2-of-3 encryption for multisig descriptors using AES-GCM.
 * Suitable for desktop usage where privacy of descriptor content is important.
 */
public class EncryptedMultisigDescriptor {

    private static final int IV_LENGTH = 12;
    private static final int TAG_LENGTH = 16;

    /**
     * Generates a symmetric AES key by hashing the sorted concatenation of two zpubs.
     */
    public static byte[] getPairKey(String zpub1, String zpub2) throws Exception {
        if (zpub1 == null || zpub2 == null || zpub1.isEmpty() || zpub2.isEmpty()) {
            throw new IllegalArgumentException("zpubs cannot be null or empty");
        }
        String[] pair = {zpub1, zpub2};
        Arrays.sort(pair);
        String concat = pair[0] + pair[1];
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        return md.digest(concat.getBytes(StandardCharsets.UTF_8));
    }

    /**
     * Encrypts a descriptor string with AES-GCM.
     */
    public static String encrypt(String descriptor, byte[] key) throws Exception {
        SecretKeySpec skey = new SecretKeySpec(key, "AES");
        Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");

        byte[] iv = new byte[IV_LENGTH];
        new SecureRandom().nextBytes(iv);
        GCMParameterSpec spec = new GCMParameterSpec(TAG_LENGTH * 8, iv);
        cipher.init(Cipher.ENCRYPT_MODE, skey, spec);

        byte[] ciphertext = cipher.doFinal(descriptor.getBytes(StandardCharsets.UTF_8));

        byte[] blob = new byte[IV_LENGTH + ciphertext.length];
        System.arraycopy(iv, 0, blob, 0, IV_LENGTH);
        System.arraycopy(ciphertext, 0, blob, IV_LENGTH, ciphertext.length);

        return Base64.getEncoder().encodeToString(blob);
    }

    /**
     * Attempts to decrypt an encrypted blob with AES-GCM and the given key.
     */
    public static String decrypt(String blobStr, byte[] key) throws Exception {
        byte[] blob = Base64.getDecoder().decode(blobStr);
        if (blob.length < IV_LENGTH) throw new IllegalArgumentException("Invalid blob length");

        byte[] iv = Arrays.copyOfRange(blob, 0, IV_LENGTH);
        byte[] ciphertext = Arrays.copyOfRange(blob, IV_LENGTH, blob.length);

        SecretKeySpec skey = new SecretKeySpec(key, "AES");
        Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");
        GCMParameterSpec spec = new GCMParameterSpec(TAG_LENGTH * 8, iv);
        cipher.init(Cipher.DECRYPT_MODE, skey, spec);

        try {
            return new String(cipher.doFinal(ciphertext), StandardCharsets.UTF_8);
        } catch (AEADBadTagException e) {
            return null;
        }
    }

    /**
     * Generates all 2-of-3 combinations of the provided zpubs.
     */
    public static List<String[]> generatePairs(String[] zpubs) {
        if (zpubs.length != 3) {
            throw new IllegalArgumentException("Exactly 3 zpubs are required");
        }

        List<String[]> pairs = new ArrayList<>();
        pairs.add(new String[]{zpubs[0], zpubs[1]});
        pairs.add(new String[]{zpubs[0], zpubs[2]});
        pairs.add(new String[]{zpubs[1], zpubs[2]});

        return pairs;
    }

    /**
     * Encrypts a descriptor using all 2-of-3 combinations of the given zpubs.
     */
    public static List<String> encrypt2of3(String descriptor, String[] zpubs) throws Exception {
        List<String[]> pairs = generatePairs(zpubs);
        List<String> blobs = new ArrayList<>();

        for (String[] pair : pairs) {
            byte[] key = getPairKey(pair[0], pair[1]);
            blobs.add(encrypt(descriptor, key));
        }

        return blobs;
    }

    /**
     * Tries to decrypt the given blob using all 2-of-3 combinations of the given zpubs.
     */
    public static String decrypt2of3(String blob, String[] zpubs) throws Exception {
        List<String[]> pairs = generatePairs(zpubs);

        for (String[] pair : pairs) {
            byte[] key = getPairKey(pair[0], pair[1]);
            String descriptor = decrypt(blob, key);
            if (descriptor != null) return descriptor;
        }

        throw new SecurityException("Decryption failed with all 2-of-3 combinations.");
    }

    /**
     * Test method demonstrating the encryption and decryption flow.
     */
    public static void demo(String[] zpubs, String descriptor) throws Exception {
        List<String> blobs = encrypt2of3(descriptor, zpubs);
        System.out.println("Encrypted blobs:");
        for (String blob : blobs) {
            System.out.println(blob);
        }

        System.out.println("\nDecryption result:");
        String decrypted = decrypt2of3(blobs.get(0), zpubs);
        System.out.println(decrypted);
    }
}
