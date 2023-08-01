#include "gtest/gtest.h"
#include <thread>

#include "../../../utils/testing.hpp"
#include "../../../number_theory/NT.hpp"
#include "../../FFT.hpp"
#include "../../NTT.hpp"
#include "naiveFFT.hpp"
#include "naiveNTT.hpp"

FFT::NTT ntt;

chrono::duration<long long, nano> expected_time_fft(ul n) {
    return (n << n) * 220ns + 5000ns;
};

void NTTcorrectAnswersCheck(const vector<int>& a, const vector<int>& b) {
    const int TRIES = 10;
    const int steps = 32 - __builtin_clz(max<int>(1, a.size()) + max<int>(1, b.size()) - 2);
    vi fast_ans = ntt.multiply(a, b, steps);
    vi indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(a.size() + b.size() - 2);
    indices.push_back(a.size() + b.size() - 1);
    indices.push_back(a.size() + b.size());
    int sz = max<int>(a.size() + b.size() - 1, 0);
    for (int index : choose_n_numbers(1, sz - 1, min(sz - 1, TRIES)))
        indices.push_back(index);
    for (int index : choose_n_numbers(sz, (1 << steps) - 1, min((1 << steps) - sz, TRIES)))
        indices.push_back(index);
    vector<thread> threads;
    auto check = [&](const int i) {
        int correct_ans = FFT::NaiveNTT::prod_nth(a, b, i);
        int f_ans = i < fast_ans.size() ? fast_ans[i] : 0;
        ASSERT_EQ(correct_ans, f_ans) << "a.size() == " << a.size() << ", b.size() == " << b.size() << ", index == " << i;
    };
    for (int index : indices)
        threads.emplace_back(check, index);
    for (thread& th : threads)
        th.join();
//    cerr << "multiply of " << a.size() << " and " << b.size() << " is correct on these indices:";
//    for (int i : indices)
//        cerr << ' ' << i;
//    cerr << '\n';
}

vi gen_int_vector(int n) {
    static mt19937 rng;
    uniform_int_distribution<int> seg(0, ntt.MOD - 1);
    vi ans(n);
    for (int i = 0; i < n; ++i)
        ans[i] = seg(rng);
    return ans;
}

void NTTcorrectAnswersCheck(const int n) {
    static mt19937 rng;
    int min_n = (1 << n - 1) + 1, max_n = 1 << n;
    for (int k = 1; k <= n; ++k) {
        int min_k = (1 << k - 1) + 1, max_k = 1 << k;
        for (int diff = 0; diff < 5; ++diff) {
            int real_k, real_n;
            if (n < ntt.n) {
                uniform_int_distribution int_k(max(min_k, max_k - diff), max_k);
                uniform_int_distribution int_n(max(min_n, max_n - diff), max_n);
                real_k = int_k(rng);
                real_n = int_n(rng);
            } else {
                uniform_int_distribution seg(1, 1 << n);
                real_k = seg(rng);
                real_n = max(1, (1 << n) + 1 - real_k - diff);
            }
            vi vk = gen_int_vector(real_k), vn = gen_int_vector(real_n);
            NTTcorrectAnswersCheck(vk, vn);
            NTTcorrectAnswersCheck(vn, vk);
        }
    }
}

#define NTTCorrectAnswersTestMacro(bits) \
TEST(NTT, NTTCorrectAnswersTest##bits) { \
    NTTcorrectAnswersCheck(bits);        \
}

NTTCorrectAnswersTestMacro(1)
NTTCorrectAnswersTestMacro(2)
NTTCorrectAnswersTestMacro(3)
NTTCorrectAnswersTestMacro(4)
NTTCorrectAnswersTestMacro(5)
NTTCorrectAnswersTestMacro(6)
NTTCorrectAnswersTestMacro(7)
NTTCorrectAnswersTestMacro(8)
NTTCorrectAnswersTestMacro(9)
NTTCorrectAnswersTestMacro(10)
NTTCorrectAnswersTestMacro(11)
NTTCorrectAnswersTestMacro(12)
NTTCorrectAnswersTestMacro(13)
NTTCorrectAnswersTestMacro(14)
NTTCorrectAnswersTestMacro(15)
NTTCorrectAnswersTestMacro(16)
NTTCorrectAnswersTestMacro(17)
NTTCorrectAnswersTestMacro(18)
NTTCorrectAnswersTestMacro(19)
NTTCorrectAnswersTestMacro(20)

void NTTFastCheck(const int bits) {
    const ul T = 3.5s / expected_time_fft(bits);

    vector<pair<vi, vi>> datasets(T);
    for (int i = 0; i < T; ++i)
        datasets[i] = {gen_int_vector(1 << bits), gen_int_vector(1 << bits)};

    ld_nano et = expected_time_fft(bits);
    vector<ld_nano> times;
    for (const auto& dataset : datasets) {
        auto start_time = chrono::high_resolution_clock::now();
        auto ans = ntt.multiply(dataset.first, dataset.second, bits + 1);
        auto end_time = chrono::high_resolution_clock::now();
        times.emplace_back(end_time - start_time);
        ASSERT_NE(ans[0], -1);
    }
    ld_nano actual_time = sample_time(times);
    cerr << "bits == " << bits << ": n log n coefficient == " << actual_time / (bits << bits) << '\n';

    cerr << "NNT multiplication for " << bits <<
         " bits (overall " << T << " tries) " <<
         " took " << actual_time <<
         " (expected " << et << ')' << endl;
    ASSERT_LE(actual_time / 1.ms, et / 1.ms);
}

#define NTTFastTestMacro(bits) \
TEST(NTT, NTTFastTest##bits) { \
    NTTFastCheck(bits);        \
}

NTTFastTestMacro(1)
NTTFastTestMacro(2)
NTTFastTestMacro(3)
NTTFastTestMacro(4)
NTTFastTestMacro(5)
NTTFastTestMacro(6)
NTTFastTestMacro(7)
NTTFastTestMacro(8)
NTTFastTestMacro(9)
NTTFastTestMacro(10)
NTTFastTestMacro(11)
NTTFastTestMacro(12)
NTTFastTestMacro(13)
NTTFastTestMacro(14)
NTTFastTestMacro(15)
NTTFastTestMacro(16)
NTTFastTestMacro(17)
NTTFastTestMacro(18)
NTTFastTestMacro(19)

void InverseCorrectAnsCheck(const int bits) {
    for (int vector_bits = 1; vector_bits <= bits; ++vector_bits)
        for (int query_bits = 1; query_bits <= bits; ++query_bits)
            for (int vector_size = (1 << vector_bits) - 1; vector_size <= min((1 << vector_bits) + 1, 1 << bits); ++vector_size)
                for (int query_size = (1 << query_bits) - 1; query_size <= min((1 << query_bits) + 1, 1 << bits); ++query_size) {
                    vi v = gen_int_vector(vector_size);
                    vi u = ntt.inverse(v, query_size);
                    ASSERT_EQ(ntt.prod(v, u, query_size), ntt.sum(vi(query_size), vi(1, 1)));
                }
}

#define InverseCorrectAnsTestMacro(bits) \
TEST(FPS, InverseCorrectAnsTest##bits) { \
    InverseCorrectAnsCheck(bits);        \
}

InverseCorrectAnsTestMacro(1)
InverseCorrectAnsTestMacro(2)
InverseCorrectAnsTestMacro(3)
InverseCorrectAnsTestMacro(4)
InverseCorrectAnsTestMacro(5)
InverseCorrectAnsTestMacro(6)
InverseCorrectAnsTestMacro(7)
InverseCorrectAnsTestMacro(8)
InverseCorrectAnsTestMacro(9)
InverseCorrectAnsTestMacro(10)
InverseCorrectAnsTestMacro(11)
InverseCorrectAnsTestMacro(12)
InverseCorrectAnsTestMacro(13)
InverseCorrectAnsTestMacro(14)
InverseCorrectAnsTestMacro(15)
InverseCorrectAnsTestMacro(16)
InverseCorrectAnsTestMacro(17)
InverseCorrectAnsTestMacro(18)
InverseCorrectAnsTestMacro(19)

TEST(FPS, EasyLogTest) {
    vi a = {0, 1, 2, 3, 245, ntt.MOD - 2, ntt.MOD - 1};
    for (int a_1 : a) {
        vi v{1, a_1};
        const int S = 500000;
        v.resize(S);
        vi u = ntt.logarithm(v, S);
        vi expected_u(S);
        for (int i = 1; i < S; ++i)
            expected_u[i] = NT::prod(NT::binpow<int>(NT::dif(0, a_1, ntt.MOD), i, ntt.MOD), NT::inv(NT::dif(0, i, ntt.MOD), ntt.MOD), ntt.MOD);
        ASSERT_EQ(u, expected_u);
    }
}

void CorrectLogCheck(const int bits) {
    static mt19937 rng;
    const int TRIES = 10;
    uniform_int_distribution<int> seg(max((1 << bits) - 3, (1 << (bits - 1)) + 1), 1 << bits);
    for (int i = 0; i < TRIES; ++i) {
        int deg = seg(rng);
        vi v = gen_int_vector(deg);
        v[0] = 1;
        vi correct_log = FFT::NaiveNTT::logarithm(v, deg);
        vi our_log = ntt.logarithm(v, deg);
        ASSERT_EQ(correct_log, our_log);
    }
}

#define CorrectLogTestMacro(bits) \
TEST(FPS, CorrectLogTest##bits) { \
    CorrectLogCheck(bits);        \
}

CorrectLogTestMacro(1)
CorrectLogTestMacro(2)
CorrectLogTestMacro(3)
CorrectLogTestMacro(4)
CorrectLogTestMacro(5)
CorrectLogTestMacro(6)
CorrectLogTestMacro(7)
CorrectLogTestMacro(8)
CorrectLogTestMacro(9)
CorrectLogTestMacro(10)

void ExpLogCheck(const int bits) {
    const int TRIES = 10;
    vi sizes = {(1 << bits - 1) + 1, 1 << bits};
    for (int i : choose_n_numbers((1 << bits - 1) + 2, (1 << bits) - 1, min(TRIES, (1 << bits - 1) - 2)))
        sizes.push_back(i);
    for (int size : sizes) {
        vi v = gen_int_vector(size);
        v[0] = 0;
        vi v_exp = ntt.exponent(v, size);
        vi v_exp_log = ntt.logarithm(v_exp, size);
        ASSERT_EQ(v, v_exp_log);
        v[0] = 1;
        vi v_log = ntt.logarithm(v, size);
        vi v_log_exp = ntt.exponent(v_log, size);
        ASSERT_EQ(v, v_log_exp);
    }
}

#define ExpLogTestMacro(bits) \
TEST(FPS, ExpLogTest##bits) { \
    ExpLogCheck(bits);        \
}

ExpLogTestMacro(1)
ExpLogTestMacro(2)
ExpLogTestMacro(3)
ExpLogTestMacro(4)
ExpLogTestMacro(5)
ExpLogTestMacro(6)
ExpLogTestMacro(7)
ExpLogTestMacro(8)
ExpLogTestMacro(9)
ExpLogTestMacro(10)
ExpLogTestMacro(11)
ExpLogTestMacro(12)
ExpLogTestMacro(13)
ExpLogTestMacro(14)
ExpLogTestMacro(15)
ExpLogTestMacro(16)
ExpLogTestMacro(17)
ExpLogTestMacro(18)
ExpLogTestMacro(19)


// see https://oeis.org/A000088 and https://oeis.org/A001349
TEST(FPS, A000088_A001349) {
    const vi A000088 = {1, 1, 2, 4, 11, 34, 156, 1044, 12346, 274668, 12005168, 20753511, 380854347, 849549682, 209104826, 721826955, 509875705, 766808107, 374514196, 553750285};
    const vi A001349 = {0, 1, 1, 2, 6, 21, 112, 853, 11117, 261080, 11716571, 8456212, 347756584, 434613547, 901586973, 10249089, 650259133, 951293339, 356845798, 71060229};
    const vi A000088_expected = ntt.euler_transform(A001349, A001349.size());
    const vi A001349_expected = ntt.inverse_euler_transform(A000088, A000088.size());
    ASSERT_EQ(A000088, A000088_expected);
    ASSERT_EQ(A001349, A001349_expected);
}


// see https://oeis.org/A006125 and https://oeis.org/A001187
TEST(FPS, A006125_A001187) {
    const vi A006125 = {1, 1, 2, 8, 64, 1024, 32768, 2097152, 268435456, 838860732, 251622994, 114902782, 733474581, 594625599, 738708721, 289149092, 500292333, 820805556, 837180163, 730375481, 566371728, 284617744, 435609680, 769921644, 773670615, 465147789, 781282541, 979275589, 950463127, 969150046, 628015743, 289177189, 787190680, 924840078, 44444310, 149467963, 970582831, 436222071, 649008163, 670233272, 36414154, 408775058, 156102147, 592054015, 79882045, 88733850, 114905915, 210357747, 710382675, 719576665, 144281713};
    const vi A001187 = {0, 1, 1, 4, 38, 728, 26704, 1866256, 251548592, 412163774, 158488195, 768116971, 789817415, 614695818, 363873083, 41485245, 180241462, 413969949, 363445973, 24661102, 299406973, 474351044, 49855257, 170511522, 850141756, 64120997, 610834968, 155586646, 337966214, 272218789, 393049277, 424294955, 726931547, 906893821, 440508700, 171302514, 840678382, 591921613, 721633735, 312273320, 56355988, 578392755, 59765802, 771235849, 547059065, 9398822, 910625045, 285689779, 103145450, 322157649, 807799776};
    const vi A006125_expected = ntt.OGF(ntt.exponent(ntt.EGF(A001187), A001187.size()));
    const vi A001187_expected = ntt.OGF(ntt.logarithm(ntt.EGF(A006125), A006125.size()));
    const vi A001187_expected_slow = ntt.OGF(FFT::NaiveNTT::logarithm(ntt.EGF(A006125), A006125.size()));
    ASSERT_EQ(A001187_expected, A001187_expected_slow);
    ASSERT_EQ(A001187, A001187_expected_slow);
    ASSERT_EQ(A001187, A001187_expected);
    ASSERT_EQ(A006125, A006125_expected);
}

// see https://oeis.org/A001858 and https://oeis.org/A000272
TEST(FPS, A000272_A001858) {
    const vi A001858 = {1, 1, 2, 7, 38, 291, 2932, 36961, 561948, 10026505, 205608536, 774463267, 589147789, 829299664, 243375906, 66263611, 965270387, 154777431, 601662699, 929537049, 893635200, 219507261, 392236640, 775545378, 714600599, 56551872, 187837583, 189925757, 50494333, 611131417, 258806070, 413153890, 109986030, 618449809, 731781234, 408507333, 282784437, 618708931, 140390992, 763548112, 859082254, 176142073, 828502767, 993024280, 226036706, 111457543, 444641140, 371032293, 908592316, 864065766, 808048179, 802971777, 981196228, 768312808, 308616172, 734673791, 989926948, 67621474, 247455489, 816913293, 904991271, 2167470, 521428463, 211366980, 847280950, 242091031, 48799390, 302130649, 289531445, 669678823, 315719929, 81233477, 648119353, 606373918, 861744131, 490053132, 890949213, 256032831, 148885963, 507814496, 18126359, 204020899, 153961505, 50672569, 358052944, 925459403, 520667549, 975087108, 890058295, 924321286, 527770745, 345832301, 915097489, 90557772, 228678962, 868600738, 630825725, 195482578, 635847953, 415321831, 81662445, 436269442, 274708374, 164249730, 450573947, 501688794, 235062444, 310973087, 231121483, 244485637, 257399582, 217176178, 830475668, 942870543, 994710399, 51310177, 762200404, 183583777, 526282419, 915977193, 313494994, 206873782, 630893930, 133719905, 796245308, 118393366, 883372828, 287911437, 487109757, 962554649, 588914902, 471014494, 175900904, 21873791, 328602721, 772108877, 836177780, 763806833, 145411329, 187900315, 153286470, 138814069, 848491640, 461655400, 151788639, 698648246, 215491501, 663557542, 910942645, 626543774, 549885656, 672907296, 985745907, 190349226, 902390812, 807169942, 654344818, 986467838, 867038216, 725967451, 43142557, 849694545, 364515268, 15339276, 828197046, 614547118, 38081279, 193143912, 989050766, 632441240, 32572101, 561072560, 642616977, 255885044, 588516316, 691717218, 255972363, 268173587, 421065401, 546640985, 273742116, 993059651, 799494178, 427625537, 95099331, 183996389, 773835715, 179403379, 498841221, 849003539, 327089840, 667847966, 658262333, 455885065, 83279826, 614399323, 115652944, 588926366, 407903368, 727827288, 971497534, 652034123, 194357209, 402886583, 412829993, 558358775, 923096802, 818755862, 820636417, 579338549, 56100269, 299030810, 655000918, 740750892, 808044005, 590691438, 510816220, 817644708, 391491054, 283469268, 514796708, 227797901, 793897248, 920105991, 908106524, 275120852, 493148281, 754916167, 942608799, 523520176, 165743089, 320885954, 816533670, 68079526, 284249650, 191783957, 545885251, 743455791, 966538535, 958612747, 549977690, 148502288, 132666806, 241653976, 126341070, 639307644, 233097871, 677983986, 248520127, 819622602, 640164760, 304497093, 450115286, 264867239, 738301675, 945046878, 941510495, 198238125, 896297871, 487769265, 193716779, 661977709, 119121833, 283822267, 18724552, 807103289, 805867741, 621187431, 23217111, 164165913, 523551597, 60246852, 64865497, 36175679, 446049274, 381105458, 99959653, 176773560, 377945852, 860658970, 467637638, 61550447, 78082570, 94997633, 583750134, 588763614, 484401788, 867142693, 511709307, 558611328, 737115300, 288432241, 232262070, 181147916, 215272711, 978288040, 406735016, 492196048, 707195239, 539685612, 889390783, 975284942, 493772829, 749787609, 724241358, 472237539, 726738934, 802906312, 678801609, 4243138, 837355924, 852945751, 824058666, 88268018, 50452448, 215646184, 527903239, 212339513, 352517653, 857141336, 187928685, 400654961, 797268490, 992001612, 326262682, 50493000, 281270081, 822309335, 679227613, 173111589, 768307459, 722656792, 888798986, 555645512, 626570345, 604960726, 75374594, 543235801, 424659401, 719562387, 857223932, 57396726, 848771818, 146469675, 269068658, 286726308, 811893756, 932513237, 685471156, 969343799, 800528592, 923700942, 914913561, 973452126, 974853268, 499494404, 172350461, 661095701, 679179484, 348216490, 959962106, 139442609, 21226360, 190846185, 34616950, 720523103, 901326171, 820529354, 225123501, 772575697, 994556725, 864371282, 126829718, 123111554, 147915324, 824729754, 853930714, 896838304, 802752292, 984812702, 672159183, 556329380, 310137073, 302805757, 222812450, 10060220, 766510156, 100191202, 585218447};
    const vi A000272 = {0, 1, 1, 3, 16, 125, 1296, 16807, 262144, 4782969, 100000000, 361458985, 26214338, 311780402, 620835367, 907596574, 229805564, 305655011, 988403481, 987167444, 826614133, 832527204, 855919381, 919949417, 574505140, 518162616, 163529427, 741138755, 730878171, 55311403, 151005029, 177990574, 728550241, 762681263, 243799681, 445971986, 274873107, 848237627, 184936458, 946495950, 734689066, 986193292, 555702775, 533953737, 14617275, 892487976, 631985469, 387623919, 843323870, 639303360, 757496884, 136431245, 833110029, 701453745, 930112846, 382666729, 473250737, 646675638, 97397078, 645032530, 453754966, 633680628, 183768788, 828888742, 654351275, 121394674, 904600467, 344907471, 827422325, 203000532, 211215471, 180811847, 579355921, 485909291, 730998883, 786338199, 307525613, 9936805, 541969946, 238583989, 99460004, 429432714, 455807899, 91692805, 790272617, 261914135, 181485867, 606273373, 844719186, 746279691, 321605353, 914838711, 483489718, 655111264, 200374372, 8873393, 44121940, 728955706, 942201577, 68019412, 252935219, 343740040, 407123022, 613196197, 450136118, 826478675, 117053216, 275718747, 399573971, 155177225, 824151732, 258065280, 329286015, 916883811, 741207372, 818758064, 510678799, 842350870, 442208286, 478081483, 574039508, 501131839, 757090459, 370129230, 509104037, 975186786, 752109731, 517140973, 685440759, 200326746, 576296540, 209609043, 484672889, 813175346, 658295977, 556122991, 412324906, 62852460, 336350682, 397905289, 798561531, 488308108, 684953016, 101701509, 276419820, 79004130, 619592751, 357417272, 812845385, 692107650, 93095129, 94895008, 169917079, 913418234, 186073886, 252700555, 261947439, 255929549, 831273617, 235630959, 225171358, 626597023, 372364055, 429334688, 95617221, 453819450, 702904391, 208752255, 260505142, 616834160, 72517498, 487442464, 975696118, 651662017, 729281456, 48366927, 389481047, 141532290, 802959961, 401477232, 65362418, 406438465, 667696868, 669969802, 423538716, 563659218, 53829646, 457376422, 13085062, 462883220, 968029951, 564676932, 668597481, 34929733, 971457722, 602350961, 946098855, 556863896, 212694614, 922693337, 85910356, 22587050, 371195893, 482969763, 839083391, 458779343, 130115084, 701600989, 219549067, 211223125, 329891045, 824983630, 116207082, 551783077, 471160199, 135634914, 142925472, 722683200, 654097157, 193679471, 806255638, 24455983, 145954019, 643010315, 558918504, 493593972, 415249036, 889648015, 950509662, 872701720, 829006981, 753493343, 221482556, 110813637, 226342813, 14290521, 482953852, 35779300, 212826267, 564534176, 269188173, 95735463, 510335596, 23962926, 769578717, 627397672, 490220863, 466227347, 590555186, 594309151, 724577575, 507844047, 924442323, 231124057, 642389082, 486148941, 422548178, 500616555, 64414709, 800891221, 566771007, 27609720, 78755790, 759555877, 619180062, 722554710, 133961134, 118815112, 514843798, 100969913, 689830942, 143815387, 399172529, 239714378, 631467006, 466563150, 212763503, 115448800, 124662617, 210986608, 569009932, 365289686, 546289493, 148447019, 889820157, 264125920, 766502976, 40474586, 615342292, 365644285, 314794722, 680992535, 295360289, 532739865, 687304701, 478481365, 960957579, 577803308, 672990319, 158535397, 668755161, 122967234, 234098838, 448741599, 153732198, 933013750, 572372904, 291667349, 888243318, 626211993, 320812174, 15193310, 415999167, 589622362, 370724257, 799408288, 34444985, 957386292, 967452349, 669293541, 806861940, 89826168, 721948930, 782149691, 710953631, 43754091, 462967769, 939083872, 137595023, 480841177, 884480189, 208941161, 587939939, 730256979, 339415860, 368754887, 874366758, 30801201, 363313456, 604888779, 596973811, 410802763, 787736547, 369415169, 757509307, 978762810, 440566328, 688019368, 798865026, 237357864, 69110909, 945863013, 677277289, 806884801, 839477763, 314823467, 867246166, 822643245, 504353798, 328990871, 275232831, 248355886, 395937247, 279271231, 82583731, 324550681, 58460461, 585444197, 85378302, 865236747, 518338920, 811450721, 994212051, 485640727, 773503794, 796269206, 988793091, 554124689, 695939375, 378800381, 867481911, 359731686, 915194131, 724903032, 294921205, 858819431, 636402351, 586292311, 311786570};
    const vi A001858_expected = ntt.OGF(ntt.exponent(ntt.EGF(A000272), A000272.size()));
    const vi A000272_expected = ntt.OGF(ntt.logarithm(ntt.EGF(A001858), A001858.size()));
    ASSERT_EQ(A001858, A001858_expected);
    ASSERT_EQ(A000272, A000272_expected);
}

TEST(NTT, CorrectPowersTest) {
    ASSERT_TRUE(NT::is_prime(ntt.MOD));
    ASSERT_EQ(1, ntt.MOD & (1 << ntt.n) - 1);
    ASSERT_EQ(ntt.ROOT, NT::binpow(ntt.G, (ntt.MOD - 1) >> ntt.n, ntt.MOD));
    ASSERT_EQ(1, NT::binpow(ntt.ROOT, 1 << ntt.n, ntt.MOD));
    ASSERT_NE(1, NT::binpow(ntt.ROOT, 1 << ntt.n - 1, ntt.MOD));
    ASSERT_EQ(ntt.MOD - 1, NT::binpow(ntt.imaginary_unit, 2, ntt.MOD));
}

TEST(NTT, Benchmark) {

}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
