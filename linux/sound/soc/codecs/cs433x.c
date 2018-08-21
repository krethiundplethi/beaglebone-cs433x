/*
 * ALSA SoC cs433x driver
 *
 *  This driver is used by controllers which can operate in DIT (SPDI/F) where
 *  no codec is needed.  This file provides stub codec that can be used
 *  in these configurations. TI DaVinci Audio controller uses this driver.
 *
 * Author:      Andreas Fellnhofer,  <a.fellnhofer@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <sound/soc.h>
#include <sound/pcm.h>
#include <sound/initval.h>
#include <linux/of.h>

#define DRV_NAME "cs433x-dac"

#define SUPP_RATES	SNDRV_PCM_RATE_8000_192000
#define SUPP_FORMATS	(SNDRV_PCM_FMTBIT_S16_LE | \
			SNDRV_PCM_FMTBIT_S20_3LE | \
			SNDRV_PCM_FMTBIT_S24_LE)

static const struct snd_soc_dapm_widget dac_widgets[] = {
	SND_SOC_DAPM_OUTPUT("line-out"),
};

static const struct snd_soc_dapm_route dac_routes[] = {
	{ "line-out", NULL, "Playback" },
};

static struct snd_soc_codec_driver soc_codec_cs433x_dac = {
	.dapm_widgets = dac_widgets,
	.num_dapm_widgets = ARRAY_SIZE(dac_widgets),
	.dapm_routes = dac_routes,
	.num_dapm_routes = ARRAY_SIZE(dac_routes),
};

static struct snd_soc_dai_driver dac_cs433x_dai = {
	.name		= "cs433x-hifi",
	.playback 	= {
		.stream_name	= "Playback",
		.channels_min	= 2,
		.channels_max	= 2,
		.rates		= SUPP_RATES,
		.formats	= SUPP_FORMATS,
	},
};

static int cs433x_dac_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev, &soc_codec_cs433x_dac,
			&dac_cs433x_dai, 1);
}

static int cs433x_dac_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id cs433x_dac_dt_ids[] = {
	{ .compatible = "cs,cs433x-dac", },
	{ }
};
MODULE_DEVICE_TABLE(of, cs433x_dac_dt_ids);
#endif

static struct platform_driver cs433x_dac_driver = {
	.probe		= cs433x_dac_probe,
	.remove		= cs433x_dac_remove,
	.driver		= {
		.name	= DRV_NAME,
		.of_match_table = of_match_ptr(cs433x_dac_dt_ids),
	},
};

module_platform_driver(cs433x_dac_driver);

MODULE_AUTHOR("Andreas Fellnhofer <a.fellnhofer@gmail.com>");
MODULE_DESCRIPTION("cs433x codec driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
