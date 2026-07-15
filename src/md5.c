/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 10:55:45 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/14 11:16:26 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/* Rotation circulaire à gauche de x de c bits.
 * Les bits qui sortent à gauche reviennent à droite.
 */
#define ROTL(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

/* Nombre de bits de rotation à appliquer après chaque opération.
 * Chaque tour possède sa propre valeur de rotation.
 */static const uint32_t	g_k[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

/* Traite un seul bloc de 512 bits (64 octets).
 * Cette fonction réalise les 64 opérations de MD5 et met à jour
 * les quatre registres A, B, C et D.
 */
static const uint32_t	g_s[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

/* Traite un bloc de 512 bits et met a jour l'etat (4 mots de 32 bits) */
static void	md5_block(uint32_t st[4], const uint8_t block[64])
{
	uint32_t	m[16];
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
	uint32_t	d;
	uint32_t	f;
	int			i;
	int			g;

	i = 0;
	/* Découpe les 64 octets en 16 mots de 32 bits. */
	while (i < 16)
	{
		m[i] = (uint32_t)block[i * 4] | ((uint32_t)block[i * 4 + 1] << 8)
			| ((uint32_t)block[i * 4 + 2] << 16)
			| ((uint32_t)block[i * 4 + 3] << 24);
		i++;
	}
	a = st[0];
	b = st[1];
	c = st[2];
	d = st[3];
	i = 0;
	while (i < 64)
	{
		if (i < 16)
		{
			f = (b & c) | (~b & d);
			g = i;
		}
		else if (i < 32)
		{
			f = (d & b) | (~d & c);
			g = (5 * i + 1) % 16;
		}
		else if (i < 48)
		{
			f = b ^ c ^ d;
			g = (3 * i + 5) % 16;
		}
		else
		{
			f = c ^ (b | ~d);
			g = (7 * i) % 16;
		}
		f = f + a + g_k[i] + m[g];
		a = d;
		d = c;
		c = b;
		b = b + ROTL(f, g_s[i]);
		i++;
	}
	st[0] += a;
	st[1] += b;
	st[2] += c;
	st[3] += d;
}

/* Construit le message rembourre. Renvoie un buffer malloc + sa taille. */
static uint8_t	*md5_pad(const uint8_t *msg, size_t len, size_t *out_len)
{
	uint8_t		*buf;
	uint64_t	bits;
	size_t		padded;
	size_t		i;

	padded = (((len + 8) / 64) + 1) * 64;
	buf = malloc(padded);
	if (!buf)
		return (NULL);
	i = 0;
	while (i < len)
	{
		buf[i] = msg[i];
		i++;
	}
	buf[len] = 0x80;
	i = len + 1;
	while (i < padded - 8)
		buf[i++] = 0;
	bits = (uint64_t)len * 8;
	i = 0;
	while (i < 8)
	{
		/* longueur en bits, sur 64 bits, en LITTLE-ENDIAN */
		buf[padded - 8 + i] = (uint8_t)(bits >> (8 * i));
		i++;
	}
	*out_len = padded;
	return (buf);
}

void	ft_md5(const uint8_t *msg, size_t len, uint8_t digest[16])
{
	uint32_t	st[4];
	uint8_t		*buf;
	size_t		padded;
	size_t		off;
	int			i;

	st[0] = 0x67452301;
	st[1] = 0xefcdab89;
	st[2] = 0x98badcfe;
	st[3] = 0x10325476;
	buf = md5_pad(msg, len, &padded);
	if (!buf)
		return ;
	off = 0;
	while (off < padded)
	{
		md5_block(st, buf + off);
		off += 64;
	}
	free(buf);
	i = 0;
	while (i < 4)
	{
		/* on ressort l'etat en LITTLE-ENDIAN */
		digest[i * 4] = (uint8_t)st[i];
		digest[i * 4 + 1] = (uint8_t)(st[i] >> 8);
		digest[i * 4 + 2] = (uint8_t)(st[i] >> 16);
		digest[i * 4 + 3] = (uint8_t)(st[i] >> 24);
		i++;
	}
}