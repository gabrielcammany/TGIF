package model;

public class Flags {

    public static final byte GROUP_ID_H = (byte) (Integer.parseInt("02", 16));
    public static final byte GROUP_ID_L = (byte) (Integer.parseInt("00", 16));

    public static final byte flag_desar = (byte) (Integer.parseInt("80", 16));
    public static final byte flag_desar_ncon = (byte) (Integer.parseInt("81", 16));
    public static final byte flag_enviar = (byte) (Integer.parseInt("82", 16));
    public static final byte flag_enviar_error = (byte) (Integer.parseInt("83", 16));
    public static final byte flag_heart = (byte) (Integer.parseInt("84", 16));
    public static final byte flag_half = (byte) (Integer.parseInt("85", 16));
    public static final byte flag_progress = (byte) (Integer.parseInt("86", 16));
    public static final byte flag_heart_data = (byte) (Integer.parseInt("87", 16));
    public static final byte flag_connection = (byte) (Integer.parseInt("88", 16));

    public static final byte confirm = (byte) (Integer.parseInt("01", 16));
    public static final byte unblock = (byte) (Integer.parseInt("89", 16));

    public static final byte ncomplex = (byte) (Integer.parseInt("01", 16));
    public static final byte complex = (byte) (Integer.parseInt("10", 16));

}
