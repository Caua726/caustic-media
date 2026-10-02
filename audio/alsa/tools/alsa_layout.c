/* audio/alsa/tools/alsa_layout.c — where ALSA struct offsets come from.
 *
 * Caustic structs are packed and C structs are not, so every struct in
 * audio/alsa/ carries explicit padding fields wherever the C ABI would have
 * inserted padding.
 *
 * This program asks the C compiler that built this machine's sound headers,
 * and its output is the reference audio/alsa/pcm.cst is written from and
 * pcm_layout_test.cst asserts against.
 *
 *   cc -o alsa_layout alsa_layout.c && ./alsa_layout > layout.txt
 */
#include <sys/ioctl.h>
#include <stdio.h>
#include <stddef.h>
#include <sound/asound.h>

#define S(t)    printf("size %-36s %zu\n", #t, sizeof(t))
#define F(t, f) printf("off  %-36s %-24s %zu\n", #t, #f, offsetof(t, f))

int main(void) {
    printf("# audio/alsa/tools/alsa_layout.c output — x86_64 linux\n");

    S(struct snd_interval);
    F(struct snd_interval, min);
    F(struct snd_interval, max);

    S(struct snd_mask);
    F(struct snd_mask, bits);

    S(struct snd_pcm_hw_params);
    F(struct snd_pcm_hw_params, flags);
    F(struct snd_pcm_hw_params, masks);
    F(struct snd_pcm_hw_params, mres);
    F(struct snd_pcm_hw_params, intervals);
    F(struct snd_pcm_hw_params, ires);
    F(struct snd_pcm_hw_params, rmask);
    F(struct snd_pcm_hw_params, cmask);
    F(struct snd_pcm_hw_params, info);
    F(struct snd_pcm_hw_params, msbits);
    F(struct snd_pcm_hw_params, rate_num);
    F(struct snd_pcm_hw_params, rate_den);
    F(struct snd_pcm_hw_params, fifo_size);
    F(struct snd_pcm_hw_params, sync);
    F(struct snd_pcm_hw_params, reserved);

    S(struct snd_pcm_sw_params);
    F(struct snd_pcm_sw_params, tstamp_mode);
    F(struct snd_pcm_sw_params, period_step);
    F(struct snd_pcm_sw_params, sleep_min);
    F(struct snd_pcm_sw_params, avail_min);
    F(struct snd_pcm_sw_params, xfer_align);
    F(struct snd_pcm_sw_params, start_threshold);
    F(struct snd_pcm_sw_params, stop_threshold);
    F(struct snd_pcm_sw_params, silence_threshold);
    F(struct snd_pcm_sw_params, silence_size);
    F(struct snd_pcm_sw_params, boundary);

    S(struct snd_xferi);
    F(struct snd_xferi, result);
    F(struct snd_xferi, buf);
    F(struct snd_xferi, frames);

    S(struct snd_pcm_info);
    F(struct snd_pcm_info, device);
    F(struct snd_pcm_info, subdevice);
    F(struct snd_pcm_info, id);
    F(struct snd_pcm_info, name);
    F(struct snd_pcm_info, subname);
    F(struct snd_pcm_info, dev_class);
    F(struct snd_pcm_info, dev_subclass);
    F(struct snd_pcm_info, subdevices_count);
    F(struct snd_pcm_info, subdevices_avail);

    return 0;
}
