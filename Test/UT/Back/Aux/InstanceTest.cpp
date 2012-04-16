//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "QDeps/Back/Aux/Instance.hpp"

namespace QDeps
{
namespace Back
{
namespace Aux
{
namespace UT
{

using namespace boost;

class A { };
class B { };
class C
{
    int dummy;
};

template<typename T, typename TName = void>
struct Named
{
    typedef T value_type;

    explicit Named(T i)
        : i(i)
    { }

    T i;
};

BOOST_AUTO_TEST_CASE(InstancePodValue)
{
    const int i = 42;
    BOOST_CHECK_EQUAL(i, Instance<int>(i).get());
}

BOOST_AUTO_TEST_CASE(InstancePodWithValueType)
{
    const int i = 42;
    BOOST_CHECK_EQUAL(i, Instance<Named<int> >(i).get());
}

BOOST_AUTO_TEST_CASE(InstanceStringValue)
{
    const std::string s = "string";
    BOOST_CHECK_EQUAL(s, Instance<std::string>(s).get());
}

BOOST_AUTO_TEST_CASE(InstanceStringWithValueType)
{
    const std::string s = "string";
    BOOST_CHECK_EQUAL(s, Instance<Named<std::string> >(s).get());
}

BOOST_AUTO_TEST_CASE(InstanceVariantRef)
{
    C c;
    C& ref = boost::get<C&>(Instance<C>(c).get());
    BOOST_CHECK_EQUAL(&c, &ref);
}

BOOST_AUTO_TEST_CASE(InstanceVariantConstRef)
{
    C c;
    const C& constRef = boost::get<const C&>(Instance<C>(c).get());
    BOOST_CHECK_EQUAL(&c, &constRef);
}

BOOST_AUTO_TEST_CASE(InstanceVariantSharedPtr)
{
    shared_ptr<C> c(new C);
    BOOST_CHECK_EQUAL(c, boost::get<shared_ptr<C> >(Instance<C>(c).get()));
}

BOOST_AUTO_TEST_CASE(InstanceNamed)
{
    typedef Named<shared_ptr<int>, A> C1;
    typedef Named<shared_ptr<int>, B> C2;

    shared_ptr<C1> c1(new C1(make_shared<int>(42)));
    shared_ptr<C2> c2(new C2(make_shared<int>(87)));

    BOOST_CHECK((*boost::get<shared_ptr<C1> >(Instance<C1>(c1).get())->i != *boost::get<shared_ptr<C2> >(Instance<C2>(c2).get())->i));
}

BOOST_AUTO_TEST_CASE(InstanceContext)
{
    shared_ptr<C> c1(new C);
    shared_ptr<C> c2(new C);

    BOOST_CHECK((Instance<int, A>(87).get() != Instance<int, B>(42).get()));
    BOOST_CHECK((boost::get<shared_ptr<C> >(Instance<C, A>(c1).get()) != boost::get<shared_ptr<C> >(Instance<C, B>(c2).get())));
}

} // namespace UT
} // namespace Aux
} // namespace Back
} // namespace QDeps

