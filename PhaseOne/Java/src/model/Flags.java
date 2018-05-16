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
    public static final byte flag_progress_reset = (byte) (Integer.parseInt("87", 16));
    public static final byte flag_connection = (byte) (Integer.parseInt("88", 16));
    public static final byte flag_delete_info = (byte) (Integer.parseInt("8A", 16));
    public static final byte flag_data = (byte) (Integer.parseInt("8B", 16));
    public static final byte flag_speed = (byte) (Integer.parseInt("8C", 16));
    public static final byte flag_ad = (byte) (Integer.parseInt("8D", 16));

    public static final byte confirm = (byte) (Integer.parseInt("01", 16));
    public static final byte unblock = (byte) (Integer.parseInt("89", 16));

    public static final byte ncomplex_default = (byte) (Integer.parseInt("08", 16));

    public static final byte ncomplex_sin = (byte) (Integer.parseInt("01", 16));
    public static final byte ncomplex_tren = (byte) (Integer.parseInt("02", 16));
    public static final byte ncomplex_dent = (byte) (Integer.parseInt("03", 16));
    public static final byte ncomplex_trian = (byte) (Integer.parseInt("04", 16));
    public static final byte complex_random = (byte) (Integer.parseInt("10", 16));
    public static final byte complex_custom = (byte) (Integer.parseInt("11", 16));

}
